#include "selectablevaluelistdialog.h"


SelectableValueListDialog::SelectableValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent) :
	EditableValueListDialog(container, query, parent)
{}

void SelectableValueListDialog::accept()
{
	if (currentIndex().isValid())
		EditableValueListDialog::accept();
	else
		warning(tr("You must choose one of the values."));
}
