#ifndef FILESYSTEMIFILEACCESSOR_H_
#define FILESYSTEMIFILEACCESSOR_H_

#include <QtCore/QString>
#include "filesystemifileinfo.h"


FILE_SYSTEM_NS_BEGIN

class IFileAccessor
{
public:
	typedef PScopedPointer<IFileAccessor> Holder;
	typedef uchar                         value_type;
	typedef IFileInfo::size_type          size_type;
	enum OpenMode
	{
		ReadOnly  = 0x00000001,
		ReadWrite = 0x00000002,
		Open      = 0x00000004,
		Create    = 0x00000008,
		Truncate  = 0x00000010
	};
    enum Permissions
    {
        UserRead   = 0x00000001, UserWrite   = 0x00000002, UserExec   = 0x00000004,
        GroupRead  = 0x00000008, GroupWrite  = 0x00000010, GroupExec  = 0x00000020,
        OthersRead = 0x00000040, OthersWrite = 0x00000080, OthersExec = 0x00000100
    };
    enum Seek
    {
    	FromBeggining,
    	FromCurrent,
    	FromEnd
    };

public:
	virtual ~IFileAccessor();

	virtual const QString &lastError() const = 0;
	virtual int permissions() const = 0;

	virtual size_type size() = 0;
	virtual bool setPermissions(int mode) = 0;
	virtual size_type read(value_type *data, size_type size) = 0;
	virtual size_type write(const value_type *data, size_type size) = 0;
	virtual size_type seek(size_type offset, Seek from = FromCurrent) = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILEACCESSOR_H_ */
