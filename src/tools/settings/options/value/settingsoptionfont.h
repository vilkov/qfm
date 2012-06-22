#ifndef SETTINGSOPTIONFONT_H_
#define SETTINGSOPTIONFONT_H_

#include <QtGui/QFont>
#include "settingsoptionvalue.h"
#include "../settingsscope.h"


SETTINGS_NS_BEGIN

class OptionFont : public Scope
{
public:
	OptionFont(const QString &id, Option *parent, const QFont &defaultValue);

	QFont font() const;
	void setFont(const QFont &font);

protected:
    virtual void loadDefault();

protected:
    static QString toString(const QFont &font);

private:
    QFont m_defaultValue;
	OptionValue m_family;
	OptionValue m_styleName;
	OptionValue m_pointSize;
	OptionValue m_underline;
	OptionValue m_strikeOut;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTIONFONT_H_ */
