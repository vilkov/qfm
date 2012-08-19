#ifndef SETTINGSDECORATION_H_
#define SETTINGSDECORATION_H_

#include "../settingsivisibleoption.h"


SETTINGS_NS_BEGIN

class Decoration : public IVisibleOption
{
public:
	Decoration();

	virtual void restoreDefault();
	virtual bool accept();
	virtual void reject();
};

SETTINGS_NS_END

#endif /* SETTINGSDECORATION_H_ */
