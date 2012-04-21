#ifndef CONFIGOPTIONCONSTRAINTLESSTHAN_H_
#define CONFIGOPTIONCONSTRAINTLESSTHAN_H_

#include <QCoreApplication>
#include "configoptionconstraint.h"
#include "../configoptions/configoptionvalue.h"


class ConfigOptionConstraintLessThan : public ConfigOptionConstraint
{
	Q_DECLARE_TR_FUNCTIONS(ConfigOptionConstraintLessThan)

public:
	ConfigOptionConstraintLessThan(ConfigOptionValue *option1, ConfigOptionValue *option2);
	virtual bool check(QWidget *parent) const;

private:
	ConfigOptionValue *m_option1;
	ConfigOptionValue *m_option2;
};

#endif /* CONFIGOPTIONCONSTRAINTLESSTHAN_H_ */
