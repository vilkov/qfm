/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
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
