#include "configoptionconstraintempty.h"
#include <QMessageBox>


ConfigOptionConstraintEmpty::ConfigOptionConstraintEmpty(ConfigOptionValue *option) :
	m_option(option)
{
	Q_ASSERT_X(m_option != 0, "ConfigOptionConstraintEmpty::ConfigOptionConstraintEmpty", "option == 0");
}

bool ConfigOptionConstraintEmpty::check(QWidget *parent) const
{
	if (m_option->editorValue().isNull())
	{
		QMessageBox::warning(parent, tr("Invalid value"), tr("Field \"%1\" can not be empty.").arg(m_option->title()));
		return false;
	}
	else
		return true;
}
