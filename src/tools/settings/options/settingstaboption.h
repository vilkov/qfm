#ifndef SETTINGSTABOPTION_H_
#define SETTINGSTABOPTION_H_

#include "settingsoption.h"


SETTINGS_NS_BEGIN

class TabOption : public Option
{
public:
	TabOption(const QString &label, Option *parent = 0) :
		Option(label, parent)
	{}

	virtual Type type() const;
};

SETTINGS_NS_END

#endif /* SETTINGSTABOPTION_H_ */
