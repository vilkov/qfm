#ifndef STATICVALUELISTDIALOG_H_
#define STATICVALUELISTDIALOG_H_

#include "../editable/widgets/editablevaluelistwidget.h"
#include "../../../../../../tools/widgets/nestedplaindialog/nestedplaindialog.h"


using namespace FileSystem::Plugins::Idm;

class StaticValueListDialog : public NestedPlainDialog
{
	Q_OBJECT

public:
	StaticValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent = 0);

	IdmEntityValue::Holder takeValue();
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

#endif /* STATICVALUELISTDIALOG_H_ */
