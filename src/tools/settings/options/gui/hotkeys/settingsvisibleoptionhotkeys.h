#ifndef SETTINGSVISIBLEOPTIONHOTKEYS_H_
#define SETTINGSVISIBLEOPTIONHOTKEYS_H_

#include "settingsvisibleoptionhotkey.h"
#include "../settingsivisibleoption.h"
#include "../../settingsscope.h"


SETTINGS_NS_BEGIN

class VisibleOptionHotKeys : public Scope, public IVisibleOption
{
public:
	VisibleOptionHotKeys(const QString &id, Option *parent);

	void manage(VisibleOptionHotKey *option) { Scope::manage(option); }

	virtual QLayout *createEditor() const;
	virtual bool accept() const;
	virtual void reject() const;
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLEOPTIONHOTKEYS_H_ */
