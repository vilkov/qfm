#ifndef NEWCOMPOSITEVALUEDIALOG_H_
#define NEWCOMPOSITEVALUEDIALOG_H_

#include <QtCore/QStringList>
#include <QtGui/QDialog>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
//#include "model/valuestreemodel.h"
#include "../../../../containeres/idmcontainer.h"
#include "../../../../../../tools/events/imp/keyboardeventhandler.h"
#include "../../../../../../tools/events/imp/keyboardeventsource.h"


using namespace FileSystem::Plugins::Idm;

class NewCompositeValueDialog : public QDialog
{
	Q_OBJECT

public:
	NewCompositeValueDialog(const IdmContainer &container, IdmEntity *entity, QWidget *parent = 0);

    virtual void accept();

private:
    QModelIndex currentIndex() const;

private:
	typedef KeyboardEventSource<
				EventSourceBase<
					QTreeView
				>
			> TreeView;
	typedef KeyboardEventHandler<
				EventHandlerBase<
					NewCompositeValueDialog
				>
			> TreeViewHandler;

	void addValue();
	void removeValue();

private:
	IdmContainer m_container;
	IdmEntity *m_entity;
	TreeViewHandler m_handler;
	TreeView m_view;
//	ValuesTreeModel m_model;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
};

#endif /* NEWCOMPOSITEVALUEDIALOG_H_ */
