#ifndef VALUELISTDIALOG_H_
#define VALUELISTDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "../../../containeres/idmcontainer.h"
#include "../../../../../tools/events/imp/keyboardeventhandler.h"
#include "../../../../../tools/events/imp/keyboardeventsource.h"


using namespace FileSystem::Plugins::Idm;

class ValueListDialog : public QDialog
{
	Q_OBJECT

public:
	ValueListDialog(const IdmContainer &container, IdmEntity *entity, QWidget *parent = 0);

    virtual void accept();

private:
	typedef KeyboardEventSource<
				EventSourceBase<
					QTreeView
				>
			> TreeView;
	typedef KeyboardEventHandler<
				EventHandlerBase<
					ValueListDialog
				>
			> TreeViewHandler;

	void addValue();
	void removeValue();

private:
	IdmContainer m_container;
	IdmEntity *m_entity;
	TreeViewHandler m_handler;
	TreeView m_view;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
};

#endif /* VALUELISTDIALOG_H_ */
