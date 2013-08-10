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
#ifndef COLORCONFIGOPTION_H_
#define COLORCONFIGOPTION_H_

#include <QtGui/QLabel>
#include <QtGui/QColor>
#include <QtGui/QPalette>
#include <QtGui/QCheckBox>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtCore/QScopedPointer>
#include <QtCore/QCoreApplication>
#include <tools/settings/configoptions/configoptionvalue.h>


class ColorConfigOption: public ::Tools::Settings::ConfigOptionValue
{
	Q_DECLARE_TR_FUNCTIONS(ColorConfigOption)

public:
	ColorConfigOption(qint32 id, const QString &title, ConfigOption *parent = 0);

	virtual bool isEnabled() const;
	virtual void setEnabled(bool value);
	virtual QWidget *createLabel(QWidget *parent, const QFont &font);
	virtual QWidget *createEditor(QWidget *parent);
	virtual QVariant editorValue() const;
	virtual void setEditorValue(const QVariant &value);
	virtual void setLoadedEditorValue(const QVariant &value);

	void setListener(QObject *listener, const char *checkListenerSlot, const char *colorListenerSlot);

private:
	inline void setEditorValue(bool value);
	inline void setEditorValue(const QColor &value);

private:
	QCheckBox *m_label;
	QLabel *m_editor;
	QPushButton *m_button;
	QObject *m_listener;
	const char *m_checkListenerSlot;
	const char *m_colorListenerSlot;
};

#endif /* COLORCONFIGOPTION_H_ */
