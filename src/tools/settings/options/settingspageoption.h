#ifndef SETTINGSPAGEOPTION_H_
#define SETTINGSPAGEOPTION_H_

#include "settingsoption.h"


SETTINGS_NS_BEGIN

class PageOption : public Option
{
public:
	PageOption(const QString &label, Option *parent = 0) :
		Option(label, parent)
	{}

	virtual Type type() const;
};

SETTINGS_NS_END

#endif /* SETTINGSPAGEOPTION_H_ */
