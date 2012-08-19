#ifndef SETTINGSRADIOBUTTON_H_
#define SETTINGSRADIOBUTTON_H_

#include "../settingsivisibleoption.h"
#include "../../value/settingsoptionvalue.h"


SETTINGS_NS_BEGIN

class RadioButton : public OptionValue, public IVisibleOption
{
public:
	typedef QList<IVisibleOption *> Container;

public:
	RadioButton(const QString &title, const QString &id, Option *parent, bool defaultValue = false);

	const QString &title() const { return m_title; }

	bool value() const { return OptionValue::value().at(0) == QChar(L'1'); }
	void setValue(bool value) { OptionValue::setValue(value ? QChar(L'1') : QChar(L'0')); }

	virtual QWidget *createEditor();
	virtual void restoreDefault();
	virtual bool accept();
	virtual void reject();

protected:
	QString m_title;
	QWidget *m_editor;
};

SETTINGS_NS_END

#endif /* SETTINGSRADIOBUTTON_H_ */
