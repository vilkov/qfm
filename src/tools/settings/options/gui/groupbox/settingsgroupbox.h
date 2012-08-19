#ifndef SETTINGSGROUPBOX_H_
#define SETTINGSGROUPBOX_H_

#include "../settingsivisibleoption.h"
#include "../../settingsscope.h"


SETTINGS_NS_BEGIN

class GroupBox : public Scope, public IVisibleOption
{
public:
	typedef QList<IVisibleOption *> Container;

public:
	GroupBox(const QString &title, const QString &id, Option *parent);

	const QString &title() const { return m_title; }

	void manage(Option *option, IVisibleOption *gui) { Scope::manage(option); m_guis.push_back(gui); }
	void manage(IVisibleOption *gui) { m_guis.push_back(gui); }

	virtual QWidget *createEditor();
	virtual void restoreDefault();
	virtual bool accept();
	virtual void reject();

protected:
	QString m_title;
	Container m_guis;
};

SETTINGS_NS_END

#endif /* SETTINGSGROUPBOX_H_ */
