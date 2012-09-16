#ifndef IDM_LISTENTITYDIALOG_H_
#define IDM_LISTENTITYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "model/idm_entitiestreemodel.h"
#include "../../containeres/idm_container.h"
#include "../../../../tools/events/qt/imp/keyboardeventhandler.h"
#include "../../../../tools/events/qt/imp/keyboardeventsource.h"


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
