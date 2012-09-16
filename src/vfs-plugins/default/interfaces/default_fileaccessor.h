#ifndef DEFAULT_FILEACCESSOR_H_
#define DEFAULT_FILEACCESSOR_H_

#include <vfs/interfaces/vfs_ifileaccessor.h>
#include "../default_ns.h"

#ifdef Q_OS_UNIX
	typedef int FileDescriptor;
#else
	#error Unknown OS!
#endif


DEFAULT_PLUGIN_NS_BEGIN

class FileAccesor : public IFileAccessor
{
public:
	FileAccesor(const QByteArray &absoluteFilePath, int mode);
	virtual ~FileAccesor();

	bool isValid() const;

	/* IFileAccessor */
	virtual QString lastError() const;
	virtual int permissions() const;

	virtual size_type size();
	virtual bool setPermissions(int mode);
	virtual size_type read(value_type *data, size_type size);
	virtual size_type write(const value_type *data, size_type size);
	virtual size_type seek(size_type offset, Seek from = FromCurrent);

private:
	FileDescriptor m_file;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_FILEACCESSOR_H_ */
