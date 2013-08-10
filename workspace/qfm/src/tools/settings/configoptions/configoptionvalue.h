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
#ifndef CONFIGOPTIONVALUE_H_
#define CONFIGOPTIONVALUE_H_

#include <QtCore/QList>
#include <QtGui/QFont>
#include <QtGui/QWidget>
#include "configoption.h"


SETTINGS_NS_BEGIN

class ConfigOptionValue: public ConfigOption
{
public:
	ConfigOptionValue(const qint32 &id, const QString &title, const QVariant &defaultValue, ConfigOption *parent = 0) :
		ConfigOption(title, parent),
		m_id(id),
		m_defaultValue(defaultValue)
	{}

	virtual Type type() const { return Value; }
	virtual QWidget *createLabel(QWidget *parent, const QFont &font) = 0;
	virtual QWidget *createEditor(QWidget *parent) = 0;
	virtual QVariant editorValue() const = 0;
	virtual void setEditorValue(const QVariant &value) = 0;
	virtual void setLoadedEditorValue(const QVariant &value) = 0;

	const qint32 &id() const { return m_id; }
	QVariant defaultValue() const { return m_defaultValue; }

private:
	qint32 m_id;
	QVariant m_defaultValue;
};

SETTINGS_NS_END

#endif /* CONFIGOPTIONVALUE_H_ */
