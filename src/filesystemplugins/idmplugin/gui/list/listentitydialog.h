#ifndef LISTENTITYDIALOG_H_
#define LISTENTITYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "model/entitiestreemodel.h"
#include "../../containeres/idmcontainer.h"
#include "../../../../tools/events/imp/keyboardeventhandler.h"
#include "../../../../tools/events/imp/keyboardeventsource.h"


using namespace FileSystem::Plugins::Idm;

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

#endif /* LISTENTITYDIALOG_H_ */
