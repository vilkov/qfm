#ifndef VFS_IFILEINFO_H_
#define VFS_IFILEINFO_H_

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include "vfs_ifiletype.h"
#include "../../tools/pointers/pscopedpointer.h"


VFS_NS_BEGIN

class IFileInfo
{
public:
	typedef PScopedPointer<IFileInfo> Holder;
	typedef qint64                    size_type;
    enum Permissions
    {
        Read = 0x0004, Write = 0x0002, Exec = 0x0001
    };

public:
    virtual ~IFileInfo();

	virtual bool isDir() const = 0;
	virtual bool isFile() const = 0;
	virtual bool isLink() const = 0;
	virtual size_type fileSize() const = 0;
	virtual QString fileName() const = 0;
	virtual const IFileType *fileType() const = 0;
	virtual QDateTime lastModified() const = 0;
	virtual int permissions() const = 0;
};

VFS_NS_END

#endif /* VFS_IFILEINFO_H_ */
