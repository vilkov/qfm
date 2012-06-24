#ifndef SETTINGSVISIBLEOPTIONVALUE_H_
#define SETTINGSVISIBLEOPTIONVALUE_H_

#include "settingsivisibleoption.h"
#include "../value/settingsoptionvalue.h"


class QLineEdit;


SETTINGS_NS_BEGIN

class VisibleOptionValue : public OptionValue, public IVisibleOption
{
public:
	VisibleOptionValue(const QString &label, const QString &id, Option *parent = 0, const QString &defaultValue = QString());

	virtual QWidget *createEditor();
	virtual void restoreDefault();
	virtual bool accept();
	virtual void reject();

private:
	QString m_label;
	QLineEdit *m_editor;
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLEOPTIONVALUE_H_ */
