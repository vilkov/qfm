#ifndef LIBARCHIVESCANNER_H_
#define LIBARCHIVESCANNER_H_

#include <QtCore/QMutex>
#include "../libarchive_ns.h"
#include "../../../../filesystem/interfaces/filesystemifilecontainer.h"


struct archive;


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

class Scanner : public IFileContainerScanner
{
public:
	class Enumerator;

public:
	Scanner(const IFileContainer *container, IFileAccessor::Holder &file);
	virtual ~Scanner();

	/* IFileContainerScanner */
	virtual void enumerate(IEnumerator::Holder &enumerator) const;
	virtual IFileInfo *info(const QString &fileName, QString &error) const;
	virtual void scan(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const;
	virtual void refresh(Snapshot &snapshot, const volatile Flags &aborted, QString &error) const;

private:
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

LIBARCHIVE_ARC_PLUGIN_NS_END

#endif /* LIBARCHIVESCANNER_H_ */
