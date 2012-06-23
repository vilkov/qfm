#ifndef SETTINGSPAGE_H_
#define SETTINGSPAGE_H_

#include "settingstab.h"
#include "../settingsivisibleoption.h"
#include "../../settingsscope.h"


SETTINGS_NS_BEGIN

class Page : public Scope, public IVisibleOption
{
public:
	typedef QList<IVisibleOption *> Container;

public:
	Page(const QString &title, const QString &id, Option *parent);

	const QString &title() const { return m_title; }

	void manage(Option *option, IVisibleOption *gui) { Scope::manage(option); m_guis.push_back(gui); }

	virtual QLayout *createEditor();
	virtual bool accept();
	virtual void reject();

protected:
	QString m_title;
	Container m_guis;
};

SETTINGS_NS_END

#endif /* SETTINGSPAGE_H_ */
