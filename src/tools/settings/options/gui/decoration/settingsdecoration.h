#ifndef SETTINGSDECORATION_H_
#define SETTINGSDECORATION_H_

#include "../settingsivisibleoption.h"
#include "../../settingsoption.h"


SETTINGS_NS_BEGIN

class Decoration : public Option, public IVisibleOption
{
public:
	Decoration(Option *parent);

	virtual void restoreDefault();
	virtual bool accept();
	virtual void reject();

protected:
    virtual void save(QXmlStreamWriter &stream) const;
    virtual void load(QXmlStreamReader &stream);
    virtual void loadDefault();
};

SETTINGS_NS_END

#endif /* SETTINGSDECORATION_H_ */
