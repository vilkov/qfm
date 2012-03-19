#include "selectablevaluelistdialog.h"


SelectableValueListDialog::SelectableValueListDialog(const IdmContainer &container, const Select &query, NestedDialog *parent) :
	EditableValueListDialog(container, query, parent)
{}

IdmEntityValue::Holder SelectableValueListDialog::takeValue()
{
	return EditableValueListDialog::takeValue();
}

void SelectableValueListDialog::accept()
{
	if (currentIndex().isValid())
		EditableValueListDialog::accept();
	else
		warning(title(), tr("You must choose one of the values."));
}
