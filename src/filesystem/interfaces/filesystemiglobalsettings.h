#ifndef FILESYSTEMIGLOBALSETTINGS_H_
#define FILESYSTEMIGLOBALSETTINGS_H_

#include "filesystemisettings.h"


FILE_SYSTEM_NS_BEGIN

class IGlobalSettings: public ISettings
{
public:
	virtual QString id() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIGLOBALSETTINGS_H_ */
