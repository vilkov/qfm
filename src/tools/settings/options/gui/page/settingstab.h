#ifndef SETTINGSVISIBLEOPTIONTAB_H_
#define SETTINGSVISIBLEOPTIONTAB_H_

#include "../settingsivisibleoption.h"
#include "../../settingsscope.h"


SETTINGS_NS_BEGIN

class Tab : public Scope, public IVisibleOption
{
public:
	typedef QList<IVisibleOption *> Container;

public:
	Tab(const QString &label, const QString &id, Option *parent);

	const QString &label() const { return m_label; }

	void manage(Option *option, IVisibleOption *gui) { Scope::manage(option); m_guis.push_back(gui); }

	virtual QLayout *createEditor(const QFont &font);
	virtual bool accept();
	virtual void reject();

private:
	QString m_label;
	Container m_guis;
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLEOPTIONTAB_H_ */
