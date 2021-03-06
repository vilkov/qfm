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

	virtual QWidget *createEditor();
	virtual void restoreDefault();
	virtual bool accept();
	virtual void reject();

private:
	QString m_label;
	Container m_guis;
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLEOPTIONTAB_H_ */
