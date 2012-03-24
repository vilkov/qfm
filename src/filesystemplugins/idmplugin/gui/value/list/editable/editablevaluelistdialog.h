#ifndef EDITABLEVALUELISTDIALOG_H_
#define EDITABLEVALUELISTDIALOG_H_

#include "widgets/editablevaluelistwidget.h"
#include "../../../../../../tools/widgets/nestedplaindialog/nestedplaindialog.h"


using namespace FileSystem::Plugins::Idm;

class EditableValueListDialog : public NestedPlainDialog
{
	Q_OBJECT

public:
	EditableValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent = 0);

protected:
	IdmEntityValue::Holder takeValue();
    QModelIndex currentIndex() const;

private:
	typedef KeyboardEventHandler<
				EventHandlerBase<
					EditableValueListDialog
				>
			> TreeViewHandler;

	void addValue();
	void removeValue();

private:
	TreeViewHandler m_handler;
	MainEditableValueListWidget m_widget;
};

#endif /* EDITABLEVALUELISTDIALOG_H_ */
