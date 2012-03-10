#include "selectablevaluelistdialog.h"
#include <QtGui/QMessageBox>


SelectableValueListDialog::SelectableValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent) :
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
		QMessageBox::warning(this, windowTitle(), "You must choose one of the values.");
}
