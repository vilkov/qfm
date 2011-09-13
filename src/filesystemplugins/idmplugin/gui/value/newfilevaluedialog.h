#ifndef NEWFILEVALUEDIALOG_H_
#define NEWFILEVALUEDIALOG_H_

#include <QtCore/QStringList>
#include <QtGui/QDialog>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "model/valuestreemodel.h"
#include "../../containeres/idmcontainer.h"
#include "../../../../tools/events/imp/keyboardeventhandler.h"
#include "../../../../tools/events/imp/keyboardeventsource.h"


using namespace FileSystem::Plugins::Idm;

class NewFileValueDialog : public QDialog
{
	Q_OBJECT

public:
	NewFileValueDialog(const IdmContainer &container, IdmEntity *entity, const QStringList &files, QWidget *parent = 0);

    virtual void accept();

private:
	typedef KeyboardEventSource<
				EventSourceBase<
					QTreeView
				>
			> TreeView;
	typedef KeyboardEventHandler<
				EventHandlerBase<
					NewFileValueDialog
				>
			> TreeViewHandler;

	void addValue();

private:
	IdmContainer m_container;
	IdmEntity *m_entity;
	TreeViewHandler m_handler;
	TreeView m_view;
	ValuesTreeModel m_model;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
};

#endif /* NEWFILEVALUEDIALOG_H_ */
