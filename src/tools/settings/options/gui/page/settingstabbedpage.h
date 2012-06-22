#ifndef SETTINGSTABBEDPAGE_H_
#define SETTINGSTABBEDPAGE_H_

#include "settingspage.h"
#include "settingstab.h"


SETTINGS_NS_BEGIN

class TabbedPage : public Page
{
public:
	TabbedPage(const QString &title, const QString &id, Option *parent);

	void manage(Tab *option) { Page::manage(option, option); }

	virtual QLayout *createEditor(const QFont &font);
	virtual bool accept();
	virtual void reject();
};

SETTINGS_NS_END

#endif /* SETTINGSTABBEDPAGE_H_ */
