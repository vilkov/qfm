#ifndef CREATEQUERYDIALOG_H_
#define CREATEQUERYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QToolBar>
#include <QtGui/QTreeView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "model/queryentitiesmodel.h"
#include "model/queryconstraintsmodel.h"
#include "../../containeres/idmcontainer.h"
#include "../../../../tools/events/imp/keyboardeventhandler.h"
#include "../../../../tools/events/imp/keyboardeventsource.h"


using namespace FileSystem::Plugins::Idm;

class CreateQueryDialog : public QDialog
{
	Q_OBJECT

public:
	CreateQueryDialog(const IdmContainer &container, IdmEntity *entity, QWidget *parent = 0);

    virtual void accept();

private:
	enum ActionId
	{
		AddGroup,
		AddCondition
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
					CreateQueryDialog
				>
			> EntitiesTreeViewHandler;

private:
	void createEntity();

private:
	QModelIndex currentIndex1();
	QModelIndex currentIndex2();

private:
	IdmContainer m_container;
	EntitiesTreeViewHandler m_handler;
	QToolBar m_toolBar1;
	QToolBar m_toolBar2;
	EntitiesTreeView m_view;
	QueryEntitiesModel m_model;
	QTreeView m_view2;
	QueryConstraintsModel m_model2;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
	QHBoxLayout m_horizontalLayout;
	QHBoxLayout m_horizontalLayout2;
};

#endif /* CREATEQUERYDIALOG_H_ */
