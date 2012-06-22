#ifndef SETTINGSVISIBLEOPTIONFONT_H_
#define SETTINGSVISIBLEOPTIONFONT_H_

#include "settingsivisibleoption.h"
#include "../value/settingsoptionfont.h"


class QWidget;


SETTINGS_NS_BEGIN

class VisibleOptioinFont : public OptionFont, public IVisibleOption
{
public:
	VisibleOptioinFont(const QString &id, Option *parent);

	virtual QLayout *createEditor(const QFont &font);
	virtual bool accept();
	virtual void reject();

	static QFont chooseFont(QWidget *parent);
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLEOPTIONFONT_H_ */
