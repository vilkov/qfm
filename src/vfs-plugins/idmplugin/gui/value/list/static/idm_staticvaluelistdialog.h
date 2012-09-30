#ifndef IDM_STATICVALUELISTDIALOG_H_
#define IDM_STATICVALUELISTDIALOG_H_

#include "../editable/widgets/idm_editablevaluelistwidget.h"
#include "../../../../../../tools/widgets/nestedplaindialog/nestedplaindialog.h"


using namespace ::VFS::Plugins::Idm;

class StaticValueListDialog : public NestedPlainDialog
{
	Q_OBJECT

public:
	StaticValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent = 0);

	EntityValue::Holder takeValue();
    virtual void accept();

protected:
    QModelIndex currentIndex() const;

private:
	typedef KeyboardEventHandler<
				EventHandlerBase<
					StaticValueListDialog
				>
			> TreeViewHandler;

	void setFocusToFilter();

private:
	TreeViewHandler m_handler;
	MainEditableValueListWidget m_widget;
};

#endif /* IDM_STATICVALUELISTDIALOG_H_ */
