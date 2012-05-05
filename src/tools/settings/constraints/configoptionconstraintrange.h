#ifndef CONFIGOPTIONCONSTRAINTRANGE_H_
#define CONFIGOPTIONCONSTRAINTRANGE_H_

#include <QtCore/QCoreApplication>
#include "configoptionconstraint.h"
#include "../configoptions/configoptionvalue.h"


SETTINGS_NS_BEGIN

class ConfigOptionConstraintRange : public ConfigOptionConstraint
{
	Q_DECLARE_TR_FUNCTIONS(ConfigOptionConstraintRange)

public:
	ConfigOptionConstraintRange(ConfigOptionValue *option, const qint32 &min, const qint32 &max);
	virtual bool check(QWidget *parent) const;

private:
	ConfigOptionValue *m_option;
	qint32 m_min;
	qint32 m_max;
};

SETTINGS_NS_END

#endif /* CONFIGOPTIONCONSTRAINTRANGE_H_ */
