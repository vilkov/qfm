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
#ifndef SETTINGSRADIOBUTTON_H_
#define SETTINGSRADIOBUTTON_H_

#include "../settingsivisibleoption.h"
#include "../../value/settingsoptionvalue.h"


SETTINGS_NS_BEGIN

class RadioButton : public OptionValue, public IVisibleOption
{
public:
	typedef QList<IVisibleOption *> Container;

public:
	RadioButton(const QString &title, const QString &id, Option *parent, bool defaultValue = false);

	const QString &title() const { return m_title; }

	bool value() const { return OptionValue::value().at(0) == QChar(L'1'); }
	void setValue(bool value) { OptionValue::setValue(value ? QChar(L'1') : QChar(L'0')); }

	virtual QWidget *createEditor();
	virtual void restoreDefault();
	virtual bool accept();
	virtual void reject();

protected:
	QString m_title;
	QWidget *m_editor;
};

SETTINGS_NS_END

#endif /* SETTINGSRADIOBUTTON_H_ */
