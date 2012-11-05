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
#ifndef IDM_LISTENTITYDIALOG_H_
#define IDM_LISTENTITYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include <tools/events/qt/imp/keyboardeventhandler.h>
#include <tools/events/qt/imp/keyboardeventsource.h>
#include "model/idm_entitiestreemodel.h"
#include "../../containeres/idm_container.h"


using namespace ::VFS::Plugins::Idm;
using namespace ::Tools::Events;

class ListEntityDialog : public QDialog
{
	Q_OBJECT

public:
	ListEntityDialog(const IdmContainer &container, QWidget *parent = 0);

    virtual void accept();

private:
	enum ActionId
	{
		Create,
		Remove,
		AddProperty,
		RemoveProperty
	};

private Q_SLOTS:
	void actionTriggered(QAction *action);

private:
	typedef KeyboardEventSource<
				EventSourceBase<
					QTreeView
				>
			> EntitiesTreeView;
	typedef KeyboardEventHandler<
				EventHandlerBase<
					ListEntityDialog
				>
			> EntitiesTreeViewHandler;

	void removeEntity(const QModelIndex &index);
	void removeEntity();
	void insertProperty();

private:
	void createEntity();

private:
	QModelIndex currentIndex();

private:
	IdmContainer m_container;
	EntitiesTreeViewHandler m_handler;
	QToolBar m_toolBar;
	EntitiesTreeView m_view;
	EntitiesTreeModel m_model;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
};

#endif /* IDM_LISTENTITYDIALOG_H_ */
