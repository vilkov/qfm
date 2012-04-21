#ifndef CONFIGOPTIONCONSTRAINTEMAIL_H_
#define CONFIGOPTIONCONSTRAINTEMAIL_H_

#include <QRegExp>
#include <QCoreApplication>
#include "configoptionconstraint.h"
#include "../configoptions/configoptionvalue.h"


class ConfigOptionConstraintEmail : public ConfigOptionConstraint
{
	Q_DECLARE_TR_FUNCTIONS(ConfigOptionConstraintEmail)

public:
	ConfigOptionConstraintEmail(ConfigOptionValue *option);
	virtual bool check(QWidget *parent) const;
	static QString emailValidator();

private:
	ConfigOptionValue *m_option;
	QRegExp m_checker;
};

#endif /* CONFIGOPTIONCONSTRAINTEMAIL_H_ */
