#ifndef SETTINGSIVISIBLEPPTION_H_
#define SETTINGSIVISIBLEPPTION_H_

#include "../../settings.h"


class QLayout;

SETTINGS_NS_BEGIN

class IVisibleOption
{
public:
	virtual ~IVisibleOption();

	virtual QLayout *createEditor() const = 0;
	virtual bool accept() const = 0;
	virtual void reject() const = 0;
};

SETTINGS_NS_END

#endif /* SETTINGSIVISIBLEPPTION_H_ */
