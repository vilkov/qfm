#include "libarchive.h"
#include <archive.h>


ARC_PLUGIN_NS_BEGIN

LibArchive::LibArchive(IFileAccessor::Holder &file) :
	m_file(file.take()),
	m_archive(archive_read_new())
{
	archive_read_open2(m_archive, this, open, read, skip, close);
	archive_read_support_compression_all(m_archive);
	archive_read_support_format_all(m_archive);
}

LibArchive::~LibArchive()
{
	archive_read_close(m_archive);
	archive_read_finish(m_archive);
}

int	LibArchive::open(struct archive *archive, void *_client_data)
{
	return ARCHIVE_OK;
}

ssize_t LibArchive::read(struct archive *archive, void *_client_data, const void **_buffer)
{
	LibArchive *self = static_cast<LibArchive *>(_client_data);
	(*_buffer) = self->m_buffer;
	return self->m_file->read(self->m_buffer, BlockSize);
}

int64_t LibArchive::skip(struct archive *archive, void *_client_data, int64_t request)
{
	return static_cast<LibArchive *>(_client_data)->m_file->seek(request, IFileAccessor::SeekFromCurrent);
}

int	LibArchive::close(struct archive *archive, void *_client_data)
{
	return ARCHIVE_OK;
}

ARC_PLUGIN_NS_END
