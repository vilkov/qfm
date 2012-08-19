#ifndef SETTINGSPAGE_H_
#define SETTINGSPAGE_H_

#include "../groupbox/settingsgroupbox.h"


SETTINGS_NS_BEGIN

class Page : public GroupBox
{
public:
	typedef QList<Page *> Pages;

public:
	Page(const QString &title, const QString &id, Option *parent);

	const Pages &subpages() const { return m_subpages; }

	using GroupBox::manage;
	void manage(Page *page) { Scope::manage(page); m_subpages.push_back(page); }

protected:
	Pages m_subpages;
};

SETTINGS_NS_END

#endif /* SETTINGSPAGE_H_ */
