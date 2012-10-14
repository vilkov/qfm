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
#ifndef CHECKCONFIGOPTION_H_
#define CHECKCONFIGOPTION_H_

#include <QtCore/QList>
#include <QtGui/QCheckBox>
#include "../../tools/settings/configoptions/configoptionvaluedefault.h"


class CheckConfigOption: public ::Tools::Settings::ConfigOptionValueDefault
{
public:
	CheckConfigOption(qint32 id, const QString &title, ConfigOption *parent);

	virtual bool isEnabled() const;
	virtual void setEnabled(bool value);
	virtual QWidget *createEditor(QWidget *parent);
	virtual QVariant editorValue() const;
	virtual void setEditorValue(const QVariant &value);
	virtual void setLoadedEditorValue(const QVariant &value);

	void setListener(QObject *listener, const char *listenerSlot);
	void addOption(ConfigOption *option);
	void removeOption(ConfigOption *option);

private:
	inline void setEditorValue(bool value);

private:
	QCheckBox *m_editor;
	QObject *m_listener;
	const char *m_listenerSlot;
	QList<ConfigOption*> m_options;
};

#endif /* CHECKCONFIGOPTION_H_ */
