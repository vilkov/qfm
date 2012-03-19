#ifndef SELECTABLEVALUELISTDIALOG_H_
#define SELECTABLEVALUELISTDIALOG_H_

#include "../editable/editablevaluelistdialog.h"


using namespace FileSystem::Plugins::Idm;

class SelectableValueListDialog : public EditableValueListDialog
{
	Q_OBJECT

public:
	SelectableValueListDialog(const IdmContainer &container, const Select &query, NestedDialog *parent);

	IdmEntityValue::Holder takeValue() { return EditableValueListDialog::takeValue(); }
    virtual void accept();
};

#endif /* SELECTABLEVALUELISTDIALOG_H_ */
