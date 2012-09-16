#ifndef ARC_LIBARCHIVE_SCANNER_H_
#define ARC_LIBARCHIVE_SCANNER_H_

#include <QtCore/QMutex>
#include <vfs/interfaces/vfs_ifilecontainer.h>
#include "../arc_libarchive_ns.h"


struct archive;


ARC_PLUGIN_LIBARCHIVE_NS_BEGIN

class Scanner : public IFileContainerScanner
{
public:
	class Enumerator;

public:
	Scanner(const IFileContainer *container, IFileAccessor::Holder &file);
	virtual ~Scanner();

	/* IFileContainerScanner */
	virtual IEnumerator *enumerate(QString &error) const;
	virtual void scan(const ScanArguments &arguments, QString &error) const;
	virtual void search(const SearchArguments &arguments, QString &error) const;

private:
	friend class Enumerator;
	class ReadArchive
	{
	public:
		ReadArchive(IFileAccessor::value_type *buffer, const IFileAccessor::Holder &file, struct archive *archive);
		~ReadArchive();

		/* Callbacks */
		static int open(struct archive *archive, void *_client_data);
		static ssize_t read(struct archive *archive, void *_client_data, const void **_buffer);
		static int64_t skip(struct archive *archive, void *_client_data, int64_t request);
		static int close(struct archive *archive, void *_client_data);

	private:
		ReadArchive(const ReadArchive &);
		ReadArchive &operator=(const ReadArchive &);

	private:
		IFileAccessor::value_type *m_buffer;
		const IFileAccessor::Holder &m_file;
		struct archive *m_archive;
		QMutex m_mutex;
	};

private:
	const IFileContainer *m_container;
	IFileAccessor::Holder m_file;
	struct archive *m_archive;

private:
	enum { BlockSize = 16384 };
	IFileAccessor::value_type m_buffer[BlockSize];
};

ARC_PLUGIN_LIBARCHIVE_NS_END

#endif /* ARC_LIBARCHIVE_SCANNER_H_ */
