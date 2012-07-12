#ifndef EDITABLEVALUELISTDIALOG_H_
#define EDITABLEVALUELISTDIALOG_H_

#include "widgets/editablevaluelistwidget.h"
#include "../../../../../../tools/widgets/nestedplaindialog/nestedplaindialog.h"


using namespace ::VFS::Plugins::Idm;

class EditableValueListDialog : public NestedPlainDialog
{
	Q_OBJECT

public:
	EditableValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent = 0);

    virtual void accept();
	virtual void reject();

protected:
	void closeDbContext();
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
	void setFocusToFilter();

private:
	TreeViewHandler m_handler;
	MainEditableValueListWidget m_widget;
};

#endif /* EDITABLEVALUELISTDIALOG_H_ */
