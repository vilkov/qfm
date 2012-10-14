/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#include "../default_filecontainer.h"
#include "../default_fileaccessor.h"
#include "../default_copycontrol.h"
#include "../default_fileinfo.h"

#include <desktop/theme/desktop_theme.h>
#include <desktop/locale/desktop_locale.h>
#include <vfs/tools/vfs_commontools.h>
#include <vfs/interfaces/vfs_inodeview.h>
#include <xdg/xdg.h>

#include <paths.h>
#include <sys/statvfs.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

#include <QtCore/QMap>
#include <QtCore/QReadWriteLock>
#include <QtCore/QVarLengthArray>


DEFAULT_PLUGIN_NS_BEGIN

class App : public IApplication
{
public:
	App(const QString &name, const QString &genericName, const QString &description, const QByteArray &exec, const QByteArray &iconName) :
		m_name(name),
		m_genericName(genericName),
		m_description(description),
		m_exec(exec),
		m_iconName(iconName)
	{
		if (char *icon_path = xdg_icon_lookup(m_iconName, ::Desktop::Theme::Small, XdgThemeApplications, ::Desktop::Theme::current()->name()))
		{
			m_icon.addFile(QString::fromUtf8(icon_path), QSize(::Desktop::Theme::Small, ::Desktop::Theme::Small));
			free(icon_path);
		}
		else
			m_icon = Info::fileTypeIcon(XDG_MIME_TYPE_UNKNOWN, ::Desktop::Theme::Small);
	}

	virtual const QIcon &icon() const
	{
		return m_icon;
	}

	virtual const QString &name() const
	{
		return m_name;
	}

	virtual const QString &genericName() const
	{
		return m_genericName;
	}

	virtual const QString &description() const
	{
		return m_description;
	}

	virtual bool exec(const IFileContainer *container, const IFileInfo *file, QString &error) const
	{
		typedef QList<QByteArray> List;
		QByteArray workingDirectory = container->location();
		QByteArray fileName(file->fileName());

		List arguments = QByteArray(m_exec).
				replace("%d", QByteArray()).
				replace("%D", QByteArray()).
				replace("%n", QByteArray()).
				replace("%N", QByteArray()).
				replace("%k", QByteArray()).
				replace("%v", QByteArray()).
				replace("%m", QByteArray()).
				trimmed().
				split(' ');

		for (List::size_type i = 0, size = arguments.size(); i < size;)
			if (arguments.at(i).indexOf('=') != -1)
			{
				arguments.removeAt(i);
				--size;
			}
			else
			{
				arguments[i] = arguments.at(i).trimmed();

				if (arguments.at(i).indexOf("%i") != -1)
				{
					arguments[i].replace("%i", m_iconName);
					arguments.insert(i, QByteArray("--icon"));
					++i;
					++size;
				}
				else
					arguments[i].
						replace("%f", fileName).
						replace("%F", fileName).
						replace("%u", fileName).
						replace("%U", fileName).
						replace("%c", ::Desktop::Locale::current()->codec()->fromUnicode(m_name));

				++i;
			}

		if (pid_t pid = fork())
		{
			if (pid < 0)
			{
				error = Info::codec()->toUnicode(::strerror(errno));
				return false;
			}
		}
		else
		{
			QVarLengthArray<char *, 8> argv(arguments.size() + 1);

			for (List::size_type i = 0, size = arguments.size(); i < size; ++i)
				argv.data()[i] = arguments[i].data();

			argv.data()[arguments.size()] = NULL;

			setsid();
			chdir(workingDirectory);
			execvp(argv.data()[0], argv.data());
			exit(EXIT_FAILURE);
		}

		return true;
	}

private:
	QIcon m_icon;
	QString m_name;
	QString m_genericName;
	QString m_description;
	QByteArray m_exec;
	QByteArray m_iconName;
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
		LinkedList list(lockedRead(m_userCache, fileType->id()));

		if (list.isInitialized())
			return list;
		else
		{
			QWriteLocker lock(&m_cacheLock);
			list = read(m_userCache, fileType->id());

			if (list.isInitialized())
				return list;
			else
			{
				const XdgJointList *apps;
				IApplications::LinkedList res;

				if (apps = xdg_joint_list_begin(xdg_apps_lookup(fileType->id().mime)))
					res = fill(apps);

				write(m_userCache, fileType->id(), res);

				return res;
			}
		}
	}

	IApplications::LinkedList findSystemApplications(const IFileType *fileType)
	{
		LinkedList list(lockedRead(m_systemCache, fileType->id()));

		if (list.isInitialized())
			return list;
		else
		{
			QWriteLocker lock(&m_cacheLock);
			list = read(m_systemCache, fileType->id());

			if (list.isInitialized())
				return list;
			else
			{
				const XdgJointList *apps;
				IApplications::LinkedList res;

				if (apps = xdg_joint_list_begin(xdg_known_apps_lookup(fileType->id().mime)))
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
		LinkedList() :
			m_initialized(false)
		{}

		LinkedList(const IApplications::LinkedList &other) :
			IApplications::LinkedList(other),
			m_initialized(true)
		{}

		bool isInitialized() const { return m_initialized; }

	private:
		bool m_initialized;
	};

	typedef QMap<FileTypeId, LinkedList> Cache;

private:
	LinkedList read(Cache &cache, const FileTypeId &index)
	{
		return cache.value(index);
	}

	LinkedList lockedRead(Cache &cache, const FileTypeId &index)
	{
		QReadLocker lock(&m_cacheLock);
		return read(cache, index);
	}

	void write(Cache &cache, const FileTypeId &index, const IApplications::LinkedList &list)
	{
		cache.insert(index, LinkedList(list));
	}

	IApplications::LinkedList fill(const XdgJointList *apps)
	{
		const char *name = NULL;
		const char *exec = NULL;
		const char *gen_name = NULL;
		const char *comment = NULL;
		const char *icon_name = NULL;

		const XdgList *values;
		const XdgAppGroup *group;
		IApplications::LinkedList res;
		::Desktop::Locale *locale = ::Desktop::Locale::current();

		do
			if (App *&app = m_applications[QString::fromUtf8(xdg_joint_list_item_app_id(apps))])
				res.push_back(app);
			else
				if (group = xdg_app_group_lookup(xdg_joint_list_item_app(apps), "Desktop Entry"))
				{
					if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "Name", locale->lang(), locale->country(), locale->modifier())))
						name = xdg_list_item_app_group_entry_value(values);
					else
						name = NULL;

					if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "GenericName", locale->lang(), locale->country(), locale->modifier())))
						gen_name = xdg_list_item_app_group_entry_value(values);
					else
						gen_name = NULL;

					if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "Comment", locale->lang(), locale->country(), locale->modifier())))
						comment = xdg_list_item_app_group_entry_value(values);
					else
						comment = NULL;

					if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "Exec", locale->lang(), locale->country(), locale->modifier())))
						exec = xdg_list_item_app_group_entry_value(values);
					else
						exec = NULL;

					if (values = xdg_list_begin(xdg_app_localized_entry_lookup(group, "Icon", locale->lang(), locale->country(), locale->modifier())))
						icon_name = xdg_list_item_app_group_entry_value(values);
					else
						icon_name = NULL;

					res.push_back(app = new App(QString::fromUtf8(name), QString::fromUtf8(gen_name), QString::fromUtf8(comment), QByteArray(exec), QByteArray(icon_name)));
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
		return new Info(info, static_cast<const IFileContainer *>(this));
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
			errno = EEXIST;
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
