#ifndef SETTINGSWIDGETSCOPE_H_
#define SETTINGSWIDGETSCOPE_H_

#include "settingsscope.h"
#include "value/settingsoptionvalue.h"


SETTINGS_NS_BEGIN

class WidgetScope : public Scope
{
public:
	WidgetScope(const QString &id, Option *parent);

	QByteArray geometry() const { return QByteArray::fromBase64(m_geometry.value().toAscii()); }
	void setGeometry(const QByteArray &value) { m_geometry.setValue(QString::fromAscii(value.toBase64())); }

private:
	OptionValue m_geometry;
};

SETTINGS_NS_END

#endif /* SETTINGSWIDGETSCOPE_H_ */
