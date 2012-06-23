#ifndef SETTINGSDIALOGSETTINGS_H_
#define SETTINGSDIALOGSETTINGS_H_

#include <QtGui/QFont>
#include "../../options/settingswidgetscope.h"


SETTINGS_NS_BEGIN

class DialogSettings : public WidgetScope
{
public:
	DialogSettings(Option *parent);

	virtual QFont font() const = 0;

	QByteArray splitterGeometry() const { return QByteArray::fromBase64(m_splitterGeometry.value().toAscii()); }
	void setSplitterGeometry(const QByteArray &value) { m_splitterGeometry.setValue(QString::fromAscii(value.toBase64())); }

private:
	OptionValue m_splitterGeometry;
};

SETTINGS_NS_END

#endif /* SETTINGSDIALOGSETTINGS_H_ */
