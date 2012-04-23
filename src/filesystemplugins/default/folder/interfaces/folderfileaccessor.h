#ifndef FOLDERFILEACCESSOR_H_
#define FOLDERFILEACCESSOR_H_

#include "../../default_ns.h"
#include "../../../../filesystem/interfaces/filesystemifileaccessor.h"

#ifdef Q_OS_UNIX
	typedef int FileDescriptor;
#elif Q_OS_WIN
	#error Unknown OS!
#else
	#error Unknown OS!
#endif


DEFAULT_PLUGIN_NS_BEGIN

class FileAccesor : public IFileAccessor
{
public:
	FileAccesor(const QString &absoluteFilePath, int mode);
	virtual ~FileAccesor();

	bool isValid() const;

	/* IFile */
	virtual const QString &lastError() const;

	virtual size_type size();
	virtual bool seek(size_type offset);
	virtual size_type read(value_type *data, size_type size);
	virtual size_type write(const value_type *data, size_type size);

private:
	FileDescriptor m_file;
	mutable QString m_lastError;
};

DEFAULT_PLUGIN_NS_END

#endif /* FOLDERFILEACCESSOR_H_ */
