#include "libunrarscanner.h"
#include "libunrarfileinfo.h"
#include "../../../../filesystem/tools/filesystempath.h"
#include "../../../../filesystem/containers/filesystemsnapshot.h"

#include <wchar.h>
#include <libunrar/rar.hpp>
#include <libunrar/dll.hpp>
#include <libunrar/timefn.hpp>

#include <QtCore/QMap>
#include <QtCore/QDebug>


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

class WrappedNodeItem : public FileSystem::WrappedNodeItem
{
public:
	typedef QMap<QString, FileSystem::WrappedNodeItem *> Container;

public:
	WrappedNodeItem(const IFileContainer *container, const QString &fileName, const struct RARHeaderDataEx &info, WrappedNodeItem *parent) :
		FileSystem::WrappedNodeItem(container, unpSize(info), parent)
	{
		m_data.path = QString::fromWCharArray(info.FileNameW);
		m_data.fileName = fileName;
		m_data.fileSize = totalSize();
		m_data.lastModified = fileTime(info.FileTime);
	}
	virtual ~WrappedNodeItem()
	{
		qDeleteAll(m_map);
	}

	WrappedNodeItem *find(const QString &fileName) const
	{
		return static_cast<WrappedNodeItem *>(m_map.value(fileName));
	}

	void insert(const QString &fileName, WrappedNodeItem *item)
	{
		incTotalSize((m_map[fileName] = item)->totalSize());
	}

	void populateInfo()
	{
		if (m_map.isEmpty())
			info() = new Info(m_data, false);
		else
		{
			items().reserve(items().size() + m_map.size());

			for (Container::iterator i = m_map.begin(), e = m_map.end(); i != e; i = m_map.erase(i))
			{
				items().append(*i);
				static_cast<WrappedNodeItem *>(*i)->populateInfo();
			}

			m_data.fileSize += totalSize();
			info() = new Info(m_data, true);
		}
	}

private:
	inline static IFileInfo::size_type unpSize(const struct RARHeaderDataEx &info)
	{
		IFileInfo::size_type res = info.UnpSizeHigh;
		return (res << (sizeof(unsigned int) * 8)) | info.UnpSize;
	}

	inline static QDateTime fileTime(unsigned int time)
	{
		RarTime dt;
		RarLocalTime lt;

		dt.SetDos(time);
		dt.GetLocal(&lt);

		return QDateTime(QDate(lt.Year, lt.Month, lt.Day), QTime(lt.Hour, lt.Minute, lt.Second));
	}

private:
	Info::Data m_data;
	Container m_map;
};


Scanner::Scanner(const IFileContainer *container, IFileAccessor::Holder &file, const QString &fileName) :
	m_container(container),
	m_file(file.take()),
	m_fileName(fileName)
{}

Scanner::~Scanner()
{}

void Scanner::enumerate(IEnumerator::Holder &enumerator) const
{

}

IFileInfo *Scanner::info(const QString &fileName, QString &error) const
{
	return NULL;
}

void Scanner::scan(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{
	QByteArray fileName(m_container->location(m_fileName).toUtf8());

	struct RAROpenArchiveDataEx archiveData;
	memset(&archiveData, 0, sizeof(struct RAROpenArchiveDataEx));

	archiveData.ArcName = fileName.data();
	archiveData.OpenMode = RAR_OM_LIST;

	if (void *archive = RAROpenArchiveEx(&archiveData))
	{
		QMap<QString, WrappedNodeItem *> parents;
	    IFileContainer::Holder container;
		struct RARHeaderDataEx fileInfo;
	    WrappedNodeItem *parent;
	    WrappedNodeItem *entry;
	    QString fileName;
	    const wchar_t *path;
	    wchar_t *sep;
		int res;

	    while ((res = RARReadHeaderEx(archive, &fileInfo)) == 0 && !aborted)
	    {
			path = fileInfo.FileNameW;

			if ((sep = wcschr(const_cast<wchar_t *>(path), L'/')) != NULL)
			{
				(*sep) = 0;
				WrappedNodeItem *&p = parents[fileName = QString::fromWCharArray(path)];
				(*sep) = L'/';

				if (p == NULL)
					snapshot.insert(fileName, p = parent = new WrappedNodeItem(m_container, fileName, fileInfo, NULL));
				else
					parent = p;

				path = (++sep);

				while ((sep = wcschr(const_cast<wchar_t *>(path), L'/')) != NULL)
				{
					(*sep) = 0;

					if (entry = parent->find(fileName = QString::fromWCharArray(path)))
						parent = entry;
					else
					{
						parent->insert(fileName, entry = new WrappedNodeItem(m_container, fileName, fileInfo, parent));
						parent = entry;
					}

					(*sep) = L'/';
					path = (++sep);
				}

				if (!(fileName = QString::fromWCharArray(path)).isEmpty() &&
					parent->find(fileName) == NULL)
				{
					parent->insert(fileName, new WrappedNodeItem(m_container, fileName, fileInfo, parent));
				}
			}
			else
			{
				WrappedNodeItem *&p = parents[fileName = QString::fromWCharArray(path)];

				if (p == NULL)
					snapshot.insert(fileName, p = new WrappedNodeItem(m_container, fileName, fileInfo, NULL));
			}

			RARProcessFile(archive, RAR_SKIP, NULL, NULL);
	    }

		if (!aborted)
	    	if (res == ERAR_END_ARCHIVE)
				for (Snapshot::const_iterator i = snapshot.begin(), end = snapshot.end(); i != end; ++i)
					static_cast<WrappedNodeItem *>((*i).second)->populateInfo();
	    	else
	    		error = QString::fromLatin1("Some error");

		RARCloseArchive(archive);
	}
}

void Scanner::refresh(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{

}

LIBUNRAR_ARC_PLUGIN_NS_END
