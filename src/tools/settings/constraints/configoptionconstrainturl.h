#ifndef CONFIGOPTIONCONSTRAINTURL_H_
#define CONFIGOPTIONCONSTRAINTURL_H_

#include <QtCore/QCoreApplication>
#include "configoptionconstraint.h"
#include "../configoptions/configoptionvalue.h"


SETTINGS_NS_BEGIN

class ConfigOptionConstraintUrl : public ConfigOptionConstraint
{
	Q_DECLARE_TR_FUNCTIONS(ConfigOptionConstraintUrl)

public:
	ConfigOptionConstraintUrl(ConfigOptionValue *option);
	virtual bool check(QWidget *parent) const;

private:
	ConfigOptionValue *m_option;
};

SETTINGS_NS_END

#endif /* CONFIGOPTIONCONSTRAINTURL_H_ */
