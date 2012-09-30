#ifndef IDM_EDITABLEVALUELISTDIALOG_H_
#define IDM_EDITABLEVALUELISTDIALOG_H_

#include "widgets/idm_editablevaluelistwidget.h"
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
	EntityValue::Holder takeValue();
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

#endif /* IDM_EDITABLEVALUELISTDIALOG_H_ */
