#include "configoptionconstraintrange.h"

#include <QtGui/QMessageBox>


SETTINGS_NS_BEGIN

ConfigOptionConstraintRange::ConfigOptionConstraintRange(ConfigOptionValue *option, const qint32 &min, const qint32 &max) :
	m_option(option),
	m_min(min),
	m_max(max)
{
	Q_ASSERT_X(m_min <= m_max, "ConfigOptionConstraintRange::ConfigOptionConstraintRange", "min > max");
}

bool ConfigOptionConstraintRange::check(QWidget *parent) const
{
	bool ok;
	qint32 val = m_option->editorValue().toInt(&ok);

	if (!ok || val < m_min || val > m_max)
	{
		QMessageBox::warning(parent, tr("Out of range"), tr("Field \"%1\" must be between %2 and %3.").arg(m_option->title()).arg(m_min).arg(m_max));
		return false;
	}
	else
		return true;
}

SETTINGS_NS_END
