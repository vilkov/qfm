#ifndef SETTINGSCONTAINER_H_
#define SETTINGSCONTAINER_H_

#include "options/settingsoption.h"


SETTINGS_NS_BEGIN

class Container
{
public:
	Container();
	virtual ~Container();

protected:
	typedef QList<Option *> List;

private:
	List m_items;
};

SETTINGS_NS_END

#endif /* SETTINGSCONTAINER_H_ */
