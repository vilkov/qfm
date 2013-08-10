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
#ifndef SETTINGSVISIBLEOPTIONVALUE_H_
#define SETTINGSVISIBLEOPTIONVALUE_H_

#include "settingsivisibleoption.h"
#include "../value/settingsoptionvalue.h"


class QLineEdit;


SETTINGS_NS_BEGIN

class VisibleOptionValue : public OptionValue, public IVisibleOption
{
public:
	VisibleOptionValue(const QString &label, const QString &id, Option *parent = 0, const QString &defaultValue = QString());

	virtual QWidget *createEditor();
	virtual void restoreDefault();
	virtual bool accept();
	virtual void reject();

private:
	QString m_label;
	QLineEdit *m_editor;
};

SETTINGS_NS_END

#endif /* SETTINGSVISIBLEOPTIONVALUE_H_ */
