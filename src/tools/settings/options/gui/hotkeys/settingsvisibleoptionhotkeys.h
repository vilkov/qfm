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

	virtual QWidget *createEditor();
	virtual void restoreDefault();
	virtual bool accept();
	virtual void reject();
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLEOPTIONHOTKEYS_H_ */
