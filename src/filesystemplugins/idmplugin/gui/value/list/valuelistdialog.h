#ifndef VALUELISTDIALOG_H_
#define VALUELISTDIALOG_H_

#include <QtGui/QDialog>
#include <QtGui/QTreeView>
#include <QtGui/QVBoxLayout>
#include <QtGui/QDialogButtonBox>
#include "model/valuelistmodel.h"
#include "../../../containeres/idmcontainer.h"
#include "../../../storage/queries/idmselectquery.h"
#include "../../../../../tools/events/imp/keyboardeventhandler.h"
#include "../../../../../tools/events/imp/keyboardeventsource.h"


using namespace FileSystem::Plugins::Idm;

class ValueListDialog : public QDialog
{
	Q_OBJECT

public:
	ValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent = 0);

	IdmEntityValue *takeSelectedValue();
    virtual void accept();

protected:
    QModelIndex currentIndex() const;

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
	Select m_query;
	TreeViewHandler m_handler;
	TreeView m_view;
	ValueListModel m_model;
	QDialogButtonBox m_buttonBox;
	QVBoxLayout m_verticatLayout;
};

#endif /* VALUELISTDIALOG_H_ */
