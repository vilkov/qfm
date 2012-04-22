#ifndef FILESYSTEMIFILELOCATION_H_
#define FILESYSTEMIFILELOCATION_H_

#include <QtCore/QString>
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class IFileLocation
{
public:
	virtual ~IFileLocation();

	virtual QString location() const = 0;
	virtual QString location(const QString &fileName) const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILELOCATION_H_ */
