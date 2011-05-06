#ifndef FILESYSTEMIFILEINFO_H_
#define FILESYSTEMIFILEINFO_H_

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class IFileInfo
{
public:
	virtual ~IFileInfo() {}

	virtual bool exists() const = 0;
	virtual QString fileName() const = 0;
	virtual QString absolutePath() const = 0;
	virtual QString absoluteFilePath() const = 0;
	virtual QDateTime lastModified() const = 0;

	virtual void refresh() = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILEINFO_H_ */
