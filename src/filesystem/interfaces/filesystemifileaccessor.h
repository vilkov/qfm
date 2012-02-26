#ifndef FILESYSTEMIFILEACCESSOR_H_
#define FILESYSTEMIFILEACCESSOR_H_

#include <QtCore/QString>
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class IFileAccessor
{
public:
	typedef uchar   value_type;
	typedef quint64 size_type;
	enum OpenMode
	{
		ReadOnly  = 0x00000001,
		ReadWrite = 0x00000002,
		Open      = 0x00000004,
		Create    = 0x00000008,
		Truncate  = 0x00000010
	};

public:
	virtual ~IFileAccessor();

	virtual const QString &lastError() const = 0;

	virtual size_type size() = 0;
	virtual bool seek(size_type offset) = 0;
	virtual size_type read(value_type *data, size_type size) = 0;
	virtual size_type write(const value_type *data, size_type size) = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILEACCESSOR_H_ */
