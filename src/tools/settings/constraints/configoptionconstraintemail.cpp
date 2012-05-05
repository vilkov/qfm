#include "configoptionconstraintemail.h"

#include <QtGui/QMessageBox>


SETTINGS_NS_BEGIN

ConfigOptionConstraintEmail::ConfigOptionConstraintEmail(ConfigOptionValue *option) :
	m_option(option),
	m_checker(emailValidator())
{}

bool ConfigOptionConstraintEmail::check(QWidget *parent) const
{
	QString res = m_option->editorValue().toString();

	if (res.isEmpty() || !m_checker.exactMatch(res))
		QMessageBox::warning(parent, tr("Invalid value"), tr("Field \"%1\" value is not valid email address.").arg(m_option->title()));
	else
		return true;

	return false;
}

QString ConfigOptionConstraintEmail::emailValidator()
{
	return QString::fromLatin1("^[-A-Za-z0-9!#$%&'*+/=?^_`{|}~]+(?:\\.[-A-Za-z0-9!#$%&'*+/=?^_`{|}~]+)*@(?:[A-Za-z0-9]([-A-Za-z0-9]{0,61}[A-Za-z0-9])?\\.)+(?:[A-Za-z]{2,5})$");
}

SETTINGS_NS_END
