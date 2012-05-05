#ifndef CONFIGOPTIONCONSTRAINTS_H_
#define CONFIGOPTIONCONSTRAINTS_H_

#include <QtGui/QWidget>
#include <QtCore/QVariant>
#include "../settings_ns.h"


SETTINGS_NS_BEGIN

class ConfigOptionConstraint
{
public:
	virtual ~ConfigOptionConstraint() {}
	virtual bool check(QWidget *parent) const = 0;
};

SETTINGS_NS_END

#endif /* CONFIGOPTIONCONSTRAINTS_H_ */
