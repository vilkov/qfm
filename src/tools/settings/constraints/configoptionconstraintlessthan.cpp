#include "configoptionconstraintlessthan.h"

#include <QtGui/QMessageBox>


SETTINGS_NS_BEGIN

ConfigOptionConstraintLessThan::ConfigOptionConstraintLessThan(ConfigOptionValue *option1, ConfigOptionValue *option2) :
	m_option1(option1),
	m_option2(option2)
{
	Q_ASSERT_X(m_option1 != m_option2, "ConfigOptionConstraintLessThan::ConfigOptionConstraintLessThan", "option1 == option2");
}

bool ConfigOptionConstraintLessThan::check(QWidget *parent) const
{
	bool ok;
	qint32 val1 = m_option1->editorValue().toInt(&ok);

	if (ok)
	{
		qint32 val2 = m_option2->editorValue().toInt(&ok);

		if (ok)
			if (val1 < val2)
				return true;
			else
				QMessageBox::warning(parent, tr("Out of range"), tr("Field \"%1\" must be less than field \"%2\".").arg(m_option1->title()).arg(m_option2->title()));
		else
			QMessageBox::warning(parent, tr("Invalid value"), tr("Field \"%1\" can not be converted to integer.").arg(m_option2->title()));
	}
	else
		QMessageBox::warning(parent, tr("Invalid value"), tr("Field \"%1\" can not be converted to integer.").arg(m_option1->title()));

	return false;
}

SETTINGS_NS_END
