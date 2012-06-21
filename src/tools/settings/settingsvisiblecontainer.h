#ifndef SETTINGSVISIBLECONTAINER_H_
#define SETTINGSVISIBLECONTAINER_H_

#include "settingscontainer.h"
#include "options/gui/page/settingspage.h"
#include "dialog/settings/settingsdialogsettings.h"


class QWidget;


SETTINGS_NS_BEGIN

class VisibleContainer : public Container
{
public:
	VisibleContainer(const QString &storage);

protected:
	using Container::manage;
	void manage(Page *option);

	void exec(const QString &title, DialogSettings &settings, QWidget *parent = 0);

protected:
	typedef QList<Page *> List;
	List m_pages;
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLECONTAINER_H_ */
