#ifndef FILESYSTEMIFILE_H_
#define FILESYSTEMIFILE_H_

#include <QtCore/QString>
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class IFile
{
public:
	typedef uchar   value_type;
	typedef quint64 size_type;
	enum OpenMode
	{
		ReadOnly,
		WriteOnly
	};

public:
	virtual ~IFile() {}

	virtual QString lastError() const = 0;
	virtual size_type size() const = 0;

	virtual bool seek(size_type off) = 0;
	virtual size_type read(value_type *data, size_type size) = 0;
	virtual size_type write(const value_type *data, size_type size) = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILE_H_ */
