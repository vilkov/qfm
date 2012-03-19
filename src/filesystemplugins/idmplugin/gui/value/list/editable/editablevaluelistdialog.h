#ifndef EDITABLEVALUELISTDIALOG_H_
#define EDITABLEVALUELISTDIALOG_H_

#include <QtGui/QTreeView>
#include "model/editablevaluelistmodel.h"
#include "../../../tools/nestedplaindialog/widgets/nestedwidget.h"
#include "../../../../containeres/idmcontainer.h"
#include "../../../../storage/queries/idmselectquery.h"
#include "../../../../../../tools/events/imp/keyboardeventhandler.h"
#include "../../../../../../tools/events/imp/keyboardeventsource.h"


using namespace FileSystem::Plugins::Idm;

class EditableValueListDialog : public NestedWidget
{
	Q_OBJECT

public:
	EditableValueListDialog(const IdmContainer &container, const Select &query, NestedDialog *parent);

	/* BaseNestedWidget */
	virtual void setFocus();

protected:
	IdmEntityValue::Holder takeValue();
    QModelIndex currentIndex() const;

private:
	typedef KeyboardEventSource<
				EventSourceBase<
					QTreeView
				>
			> TreeView;
	typedef KeyboardEventHandler<
				EventHandlerBase<
					EditableValueListDialog
				>
			> TreeViewHandler;

	void addValue();
	void removeValue();
    void setCurrentIndex(const QModelIndex &index) const;
    void select(const QModelIndex &index);

private:
	IdmContainer m_container;
	IdmEntity *m_entity;
	TreeViewHandler m_handler;
	TreeView m_view;
	EditableValueListModel m_model;
};

#endif /* EDITABLEVALUELISTDIALOG_H_ */
