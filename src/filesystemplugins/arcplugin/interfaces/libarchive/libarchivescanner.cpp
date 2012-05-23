#include "libarchivescanner.h"
#include "../arcfileinfo.h"
#include "../../../../filesystem/tools/filesystempath.h"
#include "../../../../filesystem/containers/filesystemsnapshot.h"

#include <string.h>
#include <archive.h>
#include <archive_entry.h>

#include <QtCore/QMap>
#include <QtCore/QDebug>


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

class WrappedNodeItem : public FileSystem::WrappedNodeItem
{
public:
	typedef QMap<QString, FileSystem::WrappedNodeItem *> Container;

public:
	WrappedNodeItem(const IFileContainer *container, const QString &fileName, struct archive_entry *entry, WrappedNodeItem *parent) :
		FileSystem::WrappedNodeItem(container, archive_entry_size(entry), parent)
	{
		m_data.path = QByteArray(archive_entry_pathname(entry));
		m_data.fileName = fileName;
		m_data.fileSize = totalSize();
		m_data.lastModified = QDateTime::fromTime_t(archive_entry_mtime(entry));
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
	Info::Data m_data;
	Container m_map;
};


Scanner::Scanner(const IFileContainer *container, IFileAccessor::Holder &file) :
	m_container(container),
	m_file(file.take()),
	m_archive(archive_read_new())
{
	archive_read_support_compression_all(m_archive);
	archive_read_support_format_all(m_archive);
}

Scanner::~Scanner()
{
	archive_read_finish(m_archive);
}

void Scanner::enumerate(IEnumerator::Holder &enumerator) const
{}

IFileInfo *Scanner::info(const QString &fileName, QString &error) const
{
	return NULL;
}

void Scanner::scan(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{
	ReadArchive archive(const_cast<Scanner *>(this)->m_buffer, m_file, m_archive);
    QMap<QString, WrappedNodeItem *> parents;
    IFileContainer::Holder container;
    WrappedNodeItem *parent;
    WrappedNodeItem *entry;
    struct archive_entry *e;
    QString fileName;
    const char *path;
    char *sep;
    int res;

    while ((res = archive_read_next_header(m_archive, &e)) == ARCHIVE_OK && !aborted)
    {
    	if (path = archive_entry_pathname(e))
			if ((sep = strchr(const_cast<char *>(path), '/')) != NULL)
			{
				(*sep) = 0;
				WrappedNodeItem *&p = parents[fileName = QString::fromUtf8(path)];
				(*sep) = '/';

				if (p == NULL)
					snapshot.insert(fileName, p = parent = new WrappedNodeItem(m_container, fileName, e, NULL));
				else
					parent = p;

				path = (++sep);

				while ((sep = strchr(const_cast<char *>(path), '/')) != NULL)
				{
					(*sep) = 0;

					if (entry = parent->find(fileName = QString::fromUtf8(path)))
						parent = entry;
					else
					{
						parent->insert(fileName, entry = new WrappedNodeItem(m_container, fileName, e, parent));
						parent = entry;
					}

					(*sep) = '/';
					path = (++sep);
				}

				if (!(fileName = QString::fromUtf8(path)).isEmpty() &&
					parent->find(fileName) == NULL)
				{
					parent->insert(fileName, new WrappedNodeItem(m_container, fileName, e, parent));
				}
			}
			else
			{
				WrappedNodeItem *&p = parents[fileName = QString::fromUtf8(path)];

				if (p == NULL)
					snapshot.insert(fileName, p = new WrappedNodeItem(m_container, fileName, e, NULL));
			}

		archive_read_data_skip(m_archive);
    }

	if (!aborted)
    	if (res == ARCHIVE_EOF)
			for (Snapshot::const_iterator i = snapshot.begin(), end = snapshot.end(); i != end; ++i)
				static_cast<WrappedNodeItem *>((*i).second)->populateInfo();
    	else
    		error = QString::fromUtf8(archive_error_string(m_archive));
}

void Scanner::refresh(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const
{}

Scanner::ReadArchive::ReadArchive(IFileAccessor::value_type *buffer, const IFileAccessor::Holder &file, struct archive *archive) :
	m_buffer(buffer),
	m_file(file),
	m_archive(archive)
{
	m_mutex.lock();
	archive_read_open2(m_archive, this, open, read, skip, close);
}

Scanner::ReadArchive::~ReadArchive()
{
	archive_read_close(m_archive);
	m_mutex.unlock();
}

int	Scanner::ReadArchive::open(struct archive *archive, void *_client_data)
{
	static_cast<ReadArchive *>(_client_data)->m_file->seek(0, IFileAccessor::FromBeggining);
	return ARCHIVE_OK;
}

ssize_t Scanner::ReadArchive::read(struct archive *archive, void *_client_data, const void **_buffer)
{
	ReadArchive *self = static_cast<ReadArchive *>(_client_data);
	(*_buffer) = self->m_buffer;
	return self->m_file->read(self->m_buffer, BlockSize);
}

int64_t Scanner::ReadArchive::skip(struct archive *archive, void *_client_data, int64_t request)
{
	IFileAccessor *file = static_cast<ReadArchive *>(_client_data)->m_file.data();
	int64_t res = file->seek(0, IFileAccessor::FromCurrent);
	return file->seek(request, IFileAccessor::FromCurrent) - res;
}

int	Scanner::ReadArchive::close(struct archive *archive, void *_client_data)
{
	return ARCHIVE_OK;
}

LIBARCHIVE_ARC_PLUGIN_NS_END
