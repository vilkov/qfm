#include "../defaultfilecontainer.h"
#include "../defaultfileaccessor.h"
#include "../defaultcopycontrol.h"
#include "../defaultfileinfo.h"
#include "../../../../application.h"

#include <vfs/tools/vfs_commontools.h>
#include <vfs/interfaces/vfs_inodeview.h>
#include <xdg/xdg.h>

#include <sys/statvfs.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include <QtCore/QMap>
#include <QtCore/QReadWriteLock>


DEFAULT_PLUGIN_NS_BEGIN

class App : public IApplication
{
public:
	App(const QIcon &icon, const QString &name, const QString &description, const QByteArray &exec) :
		m_icon(icon),
		m_name(name),
		m_description(description),
		m_exec(exec)
	{}

	virtual const QIcon &icon() const
	{
		return m_icon;
	}

	virtual const QString &name() const
	{
		return m_name;
	}

	virtual const QString &description() const
	{
		return m_description;
	}

private:
	QIcon m_icon;
	QString m_name;
	QString m_description;
	QByteArray m_exec;
};


class AppsCache
{
public:
	AppsCache()
	{}
	~AppsCache()
	{
		qDeleteAll(m_applications);
	}

	IApplications::LinkedList findUserApplications(const IFileType *fileType)
	{
		if (IApplications::LinkedList *list = lockedRead(m_userCache, fileType->id()))
			return *list;
		else
		{
			QWriteLocker lock(&m_cacheLock);

			if (list = read(m_userCache, fileType->id()))
				return *list;
			else
			{
				const XdgJointList *apps;
				IApplications::LinkedList res;

				if (apps = xdg_joint_list_begin(xdg_apps_lookup(fileType->id().mime.toUtf8())))
					res = fill(apps);

				write(m_userCache, fileType->id(), res);

				return res;
			}
		}
	}

	IApplications::LinkedList findSystemApplications(const IFileType *fileType)
	{
		if (IApplications::LinkedList *list = lockedRead(m_systemCache, fileType->id()))
			return *list;
		else
		{
			QWriteLocker lock(&m_cacheLock);

			if (list = read(m_systemCache, fileType->id()))
				return *list;
			else
			{
				const XdgJointList *apps;
				IApplications::LinkedList res;

				if (apps = xdg_joint_list_begin(xdg_known_apps_lookup(fileType->id().mime.toUtf8())))
					res = fill(apps);

				write(m_systemCache, fileType->id(), res);

				return res;
			}
		}
	}

private:
	class LinkedList : public IApplications::LinkedList
	{
	public:
		LinkedList(const IApplications::LinkedList &other) :
			IApplications::LinkedList(other)
		{}

		~LinkedList()
		{
			qDeleteAll(*this);
		}
	};

	typedef QMap<FileTypeId, LinkedList *> Cache;

private:
	IApplications::LinkedList *read(Cache &cache, const FileTypeId &index)
	{
		return cache.value(index);
	}

	IApplications::LinkedList *lockedRead(Cache &cache, const FileTypeId &index)
	{
		QReadLocker lock(&m_cacheLock);
		return read(cache, index);
	}

	void write(Cache &cache, const FileTypeId &index, const IApplications::LinkedList &list)
	{
		QScopedPointer<LinkedList> linkedList(new LinkedList(list));
		cache.insert(index, linkedList.data());
		linkedList.take();
	}

	IApplications::LinkedList fill(const XdgJointList *apps)
	{
		QIcon icon;
		const char *name = NULL;
		const char *exec = NULL;
		const char *gen_name = NULL;

		const XdgList *values;
		const XdgAppGroup *group;
		IApplications::LinkedList res;
		DesktopEnvironment::Locale *locale = Application::locale();

		do
			if (App *&app = m_applications[QString::fromUtf8(xdg_joint_list_item_app_id(apps))])
				res.push_back(app);
			else
				if (group = xdg_app_group_lookup(xdg_joint_list_item_app(apps), "Desktop Entry"))
				{
					if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "Name", locale->lang(), locale->country(), locale->modifier())))
						name = xdg_list_item_app_group_entry_value(values);

					if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "GenericName", locale->lang(), locale->country(), locale->modifier())))
						gen_name = xdg_list_item_app_group_entry_value(values);

					if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "Exec", locale->lang(), locale->country(), locale->modifier())))
						exec = xdg_list_item_app_group_entry_value(values);

					if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "Icon", locale->lang(), locale->country(), locale->modifier())))
						icon = Application::desktopService()->applicationIcon(xdg_list_item_app_group_entry_value(values));
					else
						icon = Application::desktopService()->applicationIcon();

					res.push_back(app = new App(icon, QString::fromUtf8(name), QString::fromUtf8(gen_name), QByteArray(exec)));
				}
		while (apps = xdg_joint_list_next(apps));

		return res;
	}

private:
	QReadWriteLock m_cacheLock;
	QMap<QString, App *> m_applications;
	Cache m_userCache;
	Cache m_systemCache;
};
static AppsCache appsCache;


FileContainer::FileContainer(const QByteArray &path) :
	m_path(IFileContainer::location(Info::codec()->toUnicode(path), path)),
	m_scanner(this)
{}

bool FileContainer::isDefault() const
{
	return true;
}

IFileInfo::size_type FileContainer::freeSpace() const
{
	struct statvfs64 info;

	if (statvfs64(m_path.as<QByteArray>(), &info) == -1)
		return 0;
	else
		return info.f_bsize * info.f_bfree;
}

ICopyControl *FileContainer::createControl(INodeView *view) const
{
	return new CopyControl(view->node(), m_path);
}

const Location &FileContainer::location() const
{
	return m_path;
}

Location FileContainer::location(const IFileInfo *info) const
{
	QByteArray path(m_path);
	path.append('/').append(info->fileName().as<QByteArray>());
	return IFileContainer::location(Info::codec()->toUnicode(path), path);
}

Location FileContainer::location(const QString &fileName) const
{
	QByteArray path(m_path);
	path.append('/');
	return IFileContainer::location(Info::codec()->toUnicode(path).append(fileName), path);
}

bool FileContainer::contains(const QString &fileName) const
{
	struct stat st;
	return ::stat(QByteArray(m_path).append('/').append(Info::codec()->fromUnicode(fileName)), &st) == 0;
}

IFileInfo *FileContainer::info(const QString &fileName, QString &error) const
{
	Info info(QByteArray(m_path).append('/').append(Info::codec()->fromUnicode(fileName)), Info::Refresh());

	if (info.exists())
		return new Info(info, Info::Identify());
	else
		error = Info::codec()->toUnicode(::strerror(ENOENT));

	return NULL;
}

bool FileContainer::remove(const IFileInfo *info, QString &error) const
{
#ifdef Q_OS_WIN
	DWORD attr = GetFileAttributesW((const wchar_t*)filePath.utf16());
	if (attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_READONLY) == FILE_ATTRIBUTE_READONLY)
		SetFileAttributesW((const wchar_t*)filePath.utf16(), attr &= ~FILE_ATTRIBUTE_READONLY);
#endif
	struct stat st;
	QByteArray name = QByteArray(m_path).append('/').append(info->fileName().as<QByteArray>());

	if (::stat(name, &st) == 0)
		if (S_ISDIR(st.st_mode))
		{
			if (::rmdir(name) == 0)
				return true;
		}
		else
			if (::unlink(name) == 0)
				return true;

	error = Info::codec()->toUnicode(::strerror(errno));
	return false;
}

bool FileContainer::rename(const IFileInfo *info, const QString &fileName, QString &error) const
{
	if (::rename(QByteArray(m_path).append('/').append(info->fileName().as<QByteArray>()),
				 QByteArray(m_path).append('/').append(Info::codec()->fromUnicode(fileName))) == 0)
	{
		return true;
	}
	else
	{
		error = Info::codec()->toUnicode(::strerror(errno));
		return false;
	}
}

bool FileContainer::move(const IFileContainer *source, const IFileInfo *info, QString &error) const
{
	QByteArray destFileName = QByteArray(m_path).append('/').append(info->fileName().as<QByteArray>());
	QByteArray sourceFileName = QByteArray(source->location()).append('/').append(info->fileName().as<QByteArray>());

	if (::link(sourceFileName, destFileName) == 0)
		return true;
	else
		if (errno == EEXIST && ::unlink(destFileName) == 0 && ::link(sourceFileName, destFileName) == 0)
			return true;

	error = Info::codec()->toUnicode(::strerror(errno));
	return false;
}

IFileContainer *FileContainer::open() const
{
	return new FileContainer(m_path);
}

IFileContainer *FileContainer::open(const IFileInfo *info, QString &error) const
{
	struct stat st;
	QByteArray name = QByteArray(m_path).append('/').append(info->fileName().as<QByteArray>());

	if (::stat(name, &st) == 0)
		if (S_ISDIR(st.st_mode))
			return new FileContainer(name);
		else
			errno = ENOTDIR;

	error = Info::codec()->toUnicode(::strerror(errno));
	return NULL;
}

IFileAccessor *FileContainer::open(const IFileInfo *info, int flags, QString &error) const
{
	IFileAccessor::Holder file(new FileAccesor(QByteArray(m_path).append('/').append(info->fileName().as<QByteArray>()), flags));

	if (file)
		if (static_cast<FileAccesor *>(file.data())->isValid())
			return file.take();
		else
			error = file->lastError();
	else
		error = Info::codec()->toUnicode(::strerror(errno));

	return NULL;
}

IFileContainer *FileContainer::create(const QString &fileName, QString &error) const
{
	struct stat st;
	QByteArray name = QByteArray(m_path).append('/').append(Info::codec()->fromUnicode(fileName));

	if (::stat(name, &st) == 0)
		if (S_ISDIR(st.st_mode))
			return new FileContainer(name);
		else
			errno = ENOTDIR;
	else
		if (errno == ENOENT && ::mkdir(name, S_IRWXU | (S_IRGRP | S_IXGRP) | (S_IROTH | S_IXOTH)) == 0)
			return new FileContainer(name);

	error = Info::codec()->toUnicode(::strerror(errno));
	return NULL;
}

IFileAccessor *FileContainer::create(const QString &fileName, int flags, QString &error) const
{
	IFileAccessor::Holder file(new FileAccesor(QByteArray(m_path).append('/').append(Info::codec()->fromUnicode(fileName)), flags | FileAccesor::Create));

	if (file)
		if (static_cast<FileAccesor *>(file.data())->isValid())
			return file.take();
		else
			error = file->lastError();
	else
		error = Info::codec()->toUnicode(::strerror(errno));

	return NULL;
}

const IFileContainerScanner *FileContainer::scanner() const
{
	return &m_scanner;
}

const IApplications *FileContainer::applications() const
{
	return this;
}

FileContainer::LinkedList FileContainer::user(const IFileType *fileType) const
{
	return appsCache.findUserApplications(fileType);
}

FileContainer::LinkedList FileContainer::system(const IFileType *fileType) const
{
	return appsCache.findSystemApplications(fileType);
}

DEFAULT_PLUGIN_NS_END
