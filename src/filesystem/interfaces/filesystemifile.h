#ifndef FILESYSTEMIFILE_H_
#define FILESYSTEMIFILE_H_

#include <QtCore/QString>
#include "filesystemifileinfo.h"


FILE_SYSTEM_NS_BEGIN

class IFile : public IFileInfo
{
public:
	typedef IFileAccessor::value_type value_type;

public:
	virtual bool rename(const QString &newName, QString &error) = 0;
	virtual IFileAccessor *open(int mode, QString &error) const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILE_H_ */
