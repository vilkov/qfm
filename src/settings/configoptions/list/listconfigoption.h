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
#ifndef LISTCONFIGOPTION_H_
#define LISTCONFIGOPTION_H_

#include <QtGui/QTreeView>
#include <QtGui/QPushButton>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtCore/QScopedPointer>
#include <QtCore/QCoreApplication>
#include "model/listconfigoptionmodel.h"
#include "model/listconfigoptiondelegate.h"
#include "../../../tools/settings/configoptions/configoptionvaluedefault.h"


class ListConfigOption: public ::Tools::Settings::ConfigOptionValueDefault
{
	Q_DECLARE_TR_FUNCTIONS(ListConfigOption)

public:
	ListConfigOption(const qint32 &id, const QString &title, ConfigOption *parent);

	virtual bool isEnabled() const;
	virtual void setEnabled(bool value);
	virtual QWidget *createEditor(QWidget *parent);
	virtual QVariant editorValue() const;
	virtual void setEditorValue(const QVariant &value);
	virtual void setLoadedEditorValue(const QVariant &value);

	void setListener(QObject *listener, const char *addSlot, const char *removeSlot);
	void add(const QString &value);
	void remove();

private:
	QWidget *m_widget;
	QTreeView *m_view;
	ListConfigOptionModel *m_model;
	QObject *m_listener;
	const char *m_addSlot;
	const char *m_removeSlot;
};

#endif /* LISTCONFIGOPTION_H_ */
