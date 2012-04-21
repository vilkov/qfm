#ifndef CONFIGOPTIONCONSTRAINTEMPTY_H_
#define CONFIGOPTIONCONSTRAINTEMPTY_H_

#include <QCoreApplication>
#include "configoptionconstraint.h"
#include "../configoptions/configoptionvalue.h"


class ConfigOptionConstraintEmpty : public ConfigOptionConstraint
{
	Q_DECLARE_TR_FUNCTIONS(ConfigOptionConstraintEmpty)

public:
	ConfigOptionConstraintEmpty(ConfigOptionValue *option);
	virtual bool check(QWidget *parent) const;

private:
	ConfigOptionValue *m_option;
};

#endif /* CONFIGOPTIONCONSTRAINTEMPTY_H_ */
