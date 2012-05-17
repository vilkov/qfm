#ifndef LIBARCHIVE_H_
#define LIBARCHIVE_H_

#include "../arcplugin_ns.h"
#include "../../../filesystem/interfaces/filesystemifileaccessor.h"


struct archive;

ARC_PLUGIN_NS_BEGIN

class LibArchive
{
public:
	LibArchive(IFileAccessor::Holder &file);
	~LibArchive();

private:
	/* Callbacks */
	static int open(struct archive *archive, void *_client_data);
	static ssize_t read(struct archive *archive, void *_client_data, const void **_buffer);
	static int64_t skip(struct archive *archive, void *_client_data, int64_t request);
	static int close(struct archive *archive, void *_client_data);

private:
	IFileAccessor::Holder m_file;
	struct archive *m_archive;

private:
	enum { BlockSize = 16384 };
	IFileAccessor::value_type m_buffer[BlockSize];
};

ARC_PLUGIN_NS_END

#endif /* LIBARCHIVE_H_ */
