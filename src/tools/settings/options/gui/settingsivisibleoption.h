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
#ifndef SETTINGSIVISIBLEOPTION_H_
#define SETTINGSIVISIBLEOPTION_H_

#include "../../settings_ns.h"


class QWidget;


SETTINGS_NS_BEGIN

class IVisibleOption
{
public:
	virtual ~IVisibleOption();

	virtual QWidget *createEditor() = 0;
	virtual void restoreDefault() = 0;
	virtual bool accept() = 0;
	virtual void reject() = 0;
};

SETTINGS_NS_END

#endif /* SETTINGSIVISIBLEOPTION_H_ */
