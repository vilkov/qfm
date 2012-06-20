#ifndef SETTINGSVISIBLECONTAINER_H_
#define SETTINGSVISIBLECONTAINER_H_

#include "settingscontainer.h"
#include "options/gui/page/settingspage.h"


SETTINGS_NS_BEGIN

class VisibleContainer : public Container
{
public:
	VisibleContainer(const QString &storage);

protected:
	using Container::manage;
	void manage(Page *option);

protected:
	typedef QList<IVisibleOption *> List;
	List m_guis;
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLECONTAINER_H_ */
