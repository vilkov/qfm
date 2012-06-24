#ifndef SETTINGSIVISIBLEOPTION_H_
#define SETTINGSIVISIBLEOPTION_H_

#include "../../settings_ns.h"


class QWidget;


SETTINGS_NS_BEGIN

class IVisibleOption
{
public:
	virtual ~IVisibleOption();

	virtual QWidget *createEditor() = 0;
	virtual bool accept() = 0;
	virtual void reject() = 0;
};

SETTINGS_NS_END

#endif /* SETTINGSIVISIBLEOPTION_H_ */
