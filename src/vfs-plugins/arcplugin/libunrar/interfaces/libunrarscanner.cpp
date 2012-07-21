#include "libunrarscanner.h"
#include "libunrarfileinfo.h"

#include <vfs/tools/vfs_path.h>
#include <vfs/containers/vfs_snapshot.h>

#include <wchar.h>
#include <libunrar/rar.hpp>
#include <libunrar/dll.hpp>
#include <libunrar/timefn.hpp>

#include <QtCore/QMap>


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

inline static IFileInfo::size_type unpackedSize(const struct RARHeaderDataEx &info)
{
	IFileInfo::size_type res = info.UnpSizeHigh;
	return (res << (sizeof(unsigned int) * 8)) | info.UnpSize;
}

inline static QDateTime fromDosTime(unsigned int time)
{
	RarTime dt;
	RarLocalTime lt;

	dt.SetDos(time);
	dt.GetLocal(&lt);

	return QDateTime(QDate(lt.Year, lt.Month, lt.Day), QTime(lt.Hour, lt.Minute, lt.Second));
}


class SnapshotItem : public ::VFS::SnapshotItem
{
public:
	typedef QMap<QString, ::VFS::SnapshotItem *> Container;

public:
	SnapshotItem(const IFileContainer *container, const Location &fileName, const struct RARHeaderDataEx &info, SnapshotItem *parent) :
		::VFS::SnapshotItem(container, unpackedSize(info), parent)
	{
		m_data.path = QString::fromWCharArray(info.FileNameW);
		m_data.fileName = fileName;
		m_data.fileSize = totalSize();
		m_data.lastModified = fromDosTime(info.FileTime);
	}
	virtual ~SnapshotItem()
	{
		qDeleteAll(m_map);
	}

	SnapshotItem *find(const QString &fileName) const
	{
		return static_cast<SnapshotItem *>(m_map.value(fileName));
	}

	void insert(const QString &fileName, SnapshotItem *item)
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
				static_cast<SnapshotItem *>(*i)->populateInfo();
			}

			m_data.fileSize += totalSize();
			info() = new Info(m_data, true);
		}
	}

private:
	Info::Data m_data;
	Container m_map;
};


Scanner::Scanner(const IFileContainer *container, const QByteArray &filePath) :
	m_container(container),
	m_filePath(filePath)
{}

Scanner::~Scanner()
{}

Scanner::IEnumerator *Scanner::enumerate(QString &error) const
{
	error = tr("LibUnrar does not support enumerating.");
	return NULL;
}

void Scanner::scan(const ScanArguments &arguments, QString &error) const
{
	struct RAROpenArchiveDataEx archiveData;
	memset(&archiveData, 0, sizeof(struct RAROpenArchiveDataEx));

	archiveData.ArcName = const_cast<char *>(m_filePath.data());
	archiveData.OpenMode = RAR_OM_LIST;

	if (void *archive = RAROpenArchiveEx(&archiveData))
	{
		QMap<QString, SnapshotItem *> parents;
	    IFileContainer::Holder container;
		struct RARHeaderDataEx fileInfo;
	    SnapshotItem *parent;
	    SnapshotItem *entry;
	    QString fileName;
	    Location fileNameLocation;
	    const wchar_t *path;
	    wchar_t *sep;
		int res;

	    while ((res = RARReadHeaderEx(archive, &fileInfo)) == 0 && !arguments.aborted)
	    {
			path = fileInfo.FileNameW;

			if ((sep = wcschr(const_cast<wchar_t *>(path), L'/')) != NULL)
			{
				(*sep) = 0;
				SnapshotItem *&p = parents[fileName = QString::fromWCharArray(path)];
				(*sep) = L'/';

				if (p == NULL)
				{
					fileNameLocation = Info::location(fileName);
					arguments.snapshot.insert(fileNameLocation, p = parent = new SnapshotItem(m_container, fileNameLocation, fileInfo, NULL));
				}
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
						parent->insert(fileName, entry = new SnapshotItem(m_container, Info::location(fileName), fileInfo, parent));
						parent = entry;
					}

					(*sep) = L'/';
					path = (++sep);
				}

				if (!(fileName = QString::fromWCharArray(path)).isEmpty() &&
					parent->find(fileName) == NULL)
				{
					parent->insert(fileName, new SnapshotItem(m_container, Info::location(fileName), fileInfo, parent));
				}
			}
			else
			{
				SnapshotItem *&p = parents[fileName = QString::fromWCharArray(path)];

				if (p == NULL)
				{
					fileNameLocation = Info::location(fileName);
					arguments.snapshot.insert(fileNameLocation, p = new SnapshotItem(m_container, fileNameLocation, fileInfo, NULL));
				}
			}

			RARProcessFile(archive, RAR_SKIP, NULL, NULL);
	    }

		if (!arguments.aborted)
	    	if (res == ERAR_END_ARCHIVE)
				for (Snapshot::const_iterator i = arguments.snapshot.begin(), end = arguments.snapshot.end(); i != end; ++i)
					static_cast<SnapshotItem *>((*i).second)->populateInfo();
	    	else
	    		error = errorDescription(res);

		RARCloseArchive(archive);
	}
}

void Scanner::search(const SearchArguments &argument, QString &error) const
{

}

QString Scanner::errorDescription(int code)
{
	switch (code)
	{
		case ERAR_NO_MEMORY:
			return tr("ERAR_NO_MEMORY");

		case ERAR_BAD_DATA:
			return tr("ERAR_BAD_DATA");

		case ERAR_BAD_ARCHIVE:
			return tr("ERAR_BAD_ARCHIVE");

		case ERAR_UNKNOWN_FORMAT:
			return tr("ERAR_UNKNOWN_FORMAT");

		case ERAR_EOPEN:
			return tr("ERAR_EOPEN");

		case ERAR_ECREATE:
			return tr("ERAR_ECREATE");

		case ERAR_ECLOSE:
			return tr("ERAR_ECLOSE");

		case ERAR_EREAD:
			return tr("ERAR_EREAD");

		case ERAR_EWRITE:
			return tr("ERAR_EWRITE");

		case ERAR_SMALL_BUF:
			return tr("ERAR_SMALL_BUF");

		case ERAR_UNKNOWN:
			return tr("ERAR_UNKNOWN");

		default:
			return tr("Unknown");
	}
}

LIBUNRAR_ARC_PLUGIN_NS_END
