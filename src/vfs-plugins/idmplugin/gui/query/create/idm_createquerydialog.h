#ifndef IDM_CREATEQUERYDIALOG_H_
#define IDM_CREATEQUERYDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QToolBar>
#include <QtGui/QSplitter>
#include <QtGui/QTreeView>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "model/idm_queryentitiesmodel.h"
#include "model/idm_queryconstraintsmodel.h"
#include "model/idm_queryconstraintsdelegate.h"
#include "../../../containeres/idm_container.h"
#include "../../../storage/queries/idm_selectquery.h"
#include "../../../../../tools/events/qt/imp/mouseeventhandler.h"
#include "../../../../../tools/events/qt/imp/mouseeventsource.h"
#include "../../../../../tools/events/qt/imp/keyboardeventhandler.h"
#include "../../../../../tools/events/qt/imp/keyboardeventsource.h"


using namespace ::VFS::Plugins::Idm;
using namespace ::Tools::Events;

class CreateQueryDialog : public QDialog
{
	Q_OBJECT

public:
	CreateQueryDialog(const IdmContainer &container, IdmEntity *entity, QWidget *parent = 0);

	Select query();
    virtual void accept();

private:
	enum ActionId
	{
		AddGroup,
		AddConstraint
	};

private Q_SLOTS:
	void actionTriggered(QAction *action);

private:
	typedef KeyboardEventSource<
				MouseDoubleClickEventSource<
					EventSourceBase<
						QTreeView
					>
				>
			> EntitiesTreeView;
	typedef KeyboardEventHandler<
				MouseDoubleClickEventHandler<
					EventHandlerBase<
						CreateQueryDialog
					>
				>
			> EntitiesTreeViewHandler;

private:
	QModelIndex currentIndex1();
	QModelIndex currentIndex2();
	void addConstraint();

private:
	QVBoxLayout m_verticatLayout;
	QHBoxLayout m_horizontalLayout;
	QSplitter m_splitter;
	IdmContainer m_container;
	IdmEntity *m_entity;
	EntitiesTreeViewHandler m_handler;
	QToolBar m_toolBar1;
	QToolBar m_toolBar2;
	EntitiesTreeView m_view;
	QueryEntitiesModel m_model;
	QTreeView m_view2;
	QueryConstraintsModel m_model2;
	QueryConstraintsDelegate m_delegate2;
	QDialogButtonBox m_buttonBox;
};

#endif /* IDM_CREATEQUERYDIALOG_H_ */
