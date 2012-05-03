#ifndef FILESYSTEMISETTINGS_H_
#define FILESYSTEMISETTINGS_H_

#include <QtCore/QString>
#include <QtCore/QVariant>
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class ISettings
{
public:
	virtual ~ISettings();

	virtual void beginGroup(const QString &name) = 0;
	virtual void writeValue(const QString &name, const QVariant &value) = 0;
	virtual QVariant readValue(const QString &name, const QVariant &defaultValue = QVariant()) = 0;
	virtual void endGroup() = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMISETTINGS_H_ */
