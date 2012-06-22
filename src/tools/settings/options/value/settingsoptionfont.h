#ifndef SETTINGSOPTIONFONT_H_
#define SETTINGSOPTIONFONT_H_

#include <QtGui/QFont>
#include "settingsoptionvalue.h"
#include "../settingsscope.h"


SETTINGS_NS_BEGIN

class OptionFont : public Scope
{
public:
	OptionFont(const QString &id, Option *parent);

	QFont font() const;
	void setFont(const QFont &font);

private:
	OptionValue m_family;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTIONFONT_H_ */
