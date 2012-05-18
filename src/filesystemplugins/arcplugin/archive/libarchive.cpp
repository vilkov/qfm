#include "libarchive.h"
#include "../interfaces/arcfileinfo.h"
#include "../../../filesystem/tools/filesystempath.h"
#include "../../../filesystem/containers/filesystemsnapshot.h"

#include <string.h>
#include <archive.h>
#include <archive_entry.h>


ARC_PLUGIN_NS_BEGIN

LibArchive::LibArchive(const IFileContainer *container, IFileAccessor::Holder &file) :
	m_container(container),
	m_file(file.take()),
	m_archive(archive_read_new())
{}

LibArchive::~LibArchive()
{
	archive_read_finish(m_archive);
}

void LibArchive::enumerate(IEnumerator::Holder &enumerator) const
{}

IFileInfo *LibArchive::info(const QString &fileName, QString &error) const
{
	return NULL;
}

void LibArchive::scan(Snapshot &snapshot, const volatile Flags &aborted) const
{
	ReadArchive archive(const_cast<LibArchive *>(this)->m_buffer, m_file, m_archive);
    QMap<QString, WrappedNodeItem *> parents;
    WrappedNodeItem *parent;
    WrappedNodeItem *entry;
    IFileInfo::Holder info;
    struct archive_entry *e;
    QString fileName;
    const char *path;
    char *sep;
    int res;

    while ((res = archive_read_next_header(m_archive, &e)) == ARCHIVE_OK && !aborted)
    {
    	if (path = archive_entry_pathname(e))
		{
			if ((sep = strchr(const_cast<char *>(path), '/')) != NULL)
			{
				(*sep) = 0;
				WrappedNodeItem *&p = parents[fileName = QString::fromUtf8(path)];
				(*sep) = '/';

				if (p == NULL)
					snapshot.insert(fileName, p = parent = new WrappedNodeItem(m_container, info = new Info(fileName, e), NULL));
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
						parent->insert(fileName, entry = new WrappedNodeItem(m_container, info = new Info(fileName, e), parent));
						parent = entry;
					}

					(*sep) = '/';
					path = (++sep);
				}

				if (!(fileName = QString::fromUtf8(path)).isEmpty())
					parent->insert(fileName, new WrappedNodeItem(m_container, info = new Info(fileName, e), parent));
			}
			else
			{
				fileName = QString::fromUtf8(path);
				snapshot.insert(fileName, new WrappedNodeItem(m_container, info = new Info(fileName, e), NULL));
			}
		}

		archive_read_data_skip(m_archive);
    }

//    if (res != ARCHIVE_EOF && !aborted)
//		state->error = QString::fromUtf8(archive_error_string(m_archive));
}

void LibArchive::refresh(Snapshot &snapshot, const volatile Flags &aborted) const
{}

LibArchive::ReadArchive::ReadArchive(IFileAccessor::value_type *buffer, const IFileAccessor::Holder &file, struct archive *archive) :
	m_buffer(buffer),
	m_file(file),
	m_archive(archive)
{
	archive_read_open2(m_archive, this, open, read, skip, close);
	archive_read_support_compression_all(m_archive);
	archive_read_support_format_all(m_archive);
}

LibArchive::ReadArchive::~ReadArchive()
{
	archive_read_close(m_archive);
}

int	LibArchive::ReadArchive::open(struct archive *archive, void *_client_data)
{
	static_cast<ReadArchive *>(_client_data)->m_file->seek(0, IFileAccessor::FromBeggining);
	return ARCHIVE_OK;
}

ssize_t LibArchive::ReadArchive::read(struct archive *archive, void *_client_data, const void **_buffer)
{
	ReadArchive *self = static_cast<ReadArchive *>(_client_data);
	(*_buffer) = self->m_buffer;
	return self->m_file->read(self->m_buffer, BlockSize);
}

int64_t LibArchive::ReadArchive::skip(struct archive *archive, void *_client_data, int64_t request)
{
	return static_cast<ReadArchive *>(_client_data)->m_file->seek(request, IFileAccessor::FromCurrent);
}

int	LibArchive::ReadArchive::close(struct archive *archive, void *_client_data)
{
	return ARCHIVE_OK;
}

ARC_PLUGIN_NS_END
