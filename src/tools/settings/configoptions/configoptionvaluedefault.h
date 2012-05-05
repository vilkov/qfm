#ifndef CONFIGOPTIONVALUEDEFAULT_H_
#define CONFIGOPTIONVALUEDEFAULT_H_

#include <QtGui/QLabel>
#include "configoptionvalue.h"


SETTINGS_NS_BEGIN

class ConfigOptionValueDefault: public ConfigOptionValue
{
public:
	ConfigOptionValueDefault(const qint32 &id, const QString &title, const QVariant &defaultValue, ConfigOption *parent = 0) :
		ConfigOptionValue(id, title, defaultValue, parent)
	{}

	virtual QWidget *createLabel(QWidget *parent, const QFont &font)
	{
		QLabel *label = new QLabel(title(), parent);
		label->setFont(font);
		return label;
	}
};

SETTINGS_NS_END

#endif /* CONFIGOPTIONVALUEDEFAULT_H_ */
