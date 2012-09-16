#ifndef IDM_SELECTABLEVALUELISTDIALOG_H_
#define IDM_SELECTABLEVALUELISTDIALOG_H_

#include "../editable/idm_editablevaluelistdialog.h"


using namespace ::VFS::Plugins::Idm;

class SelectableValueListDialog : public EditableValueListDialog
{
	Q_OBJECT

public:
	SelectableValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent = 0);

	IdmEntityValue::Holder takeValue() { return EditableValueListDialog::takeValue(); }
    virtual void accept();
};

#endif /* IDM_SELECTABLEVALUELISTDIALOG_H_ */
