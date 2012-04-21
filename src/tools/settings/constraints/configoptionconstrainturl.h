#ifndef CONFIGOPTIONCONSTRAINTURL_H_
#define CONFIGOPTIONCONSTRAINTURL_H_

#include <QCoreApplication>
#include "configoptionconstraint.h"
#include "../configoptions/configoptionvalue.h"


class ConfigOptionConstraintUrl : public ConfigOptionConstraint
{
	Q_DECLARE_TR_FUNCTIONS(ConfigOptionConstraintUrl)

public:
	ConfigOptionConstraintUrl(ConfigOptionValue *option);
	virtual bool check(QWidget *parent) const;

private:
	ConfigOptionValue *m_option;
};

#endif /* CONFIGOPTIONCONSTRAINTURL_H_ */
