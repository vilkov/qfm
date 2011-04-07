#include "configoptionconstrainturl.h"
#include <QUrl>
#include <QMessageBox>


ConfigOptionConstraintUrl::ConfigOptionConstraintUrl(ConfigOptionValue *option) :
	m_option(option)
{}

bool ConfigOptionConstraintUrl::check(QWidget *parent) const
{
	if (!QUrl(m_option->editorValue().toString()).isValid())
	{
		QMessageBox::warning(parent, tr("Invalid value"), tr("Field \"%1\" value is not valid.").arg(m_option->title()));
		return false;
	}
	else
		return true;
}
