#ifndef CONFIGOPTIONCONSTRAINTEMPTY_H_
#define CONFIGOPTIONCONSTRAINTEMPTY_H_

#include <QtCore/QCoreApplication>
#include "configoptionconstraint.h"
#include "../configoptions/configoptionvalue.h"


SETTINGS_NS_BEGIN

class ConfigOptionConstraintEmpty : public ConfigOptionConstraint
{
	Q_DECLARE_TR_FUNCTIONS(ConfigOptionConstraintEmpty)

public:
	ConfigOptionConstraintEmpty(ConfigOptionValue *option);
	virtual bool check(QWidget *parent) const;

private:
	ConfigOptionValue *m_option;
};

SETTINGS_NS_END

#endif /* CONFIGOPTIONCONSTRAINTEMPTY_H_ */
