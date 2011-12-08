#include "selectablevaluelistdialog.h"
#include <QtGui/QMessageBox>


SelectableValueListDialog::SelectableValueListDialog(const IdmContainer &container, const Select &query, QWidget *parent) :
	EditableValueListDialog(container, query, parent)
{}

IdmEntityValue *SelectableValueListDialog::takeValue()
{
	return EditableValueListDialog::takeValue();
}

void SelectableValueListDialog::accept()
{
	if (currentIndex().isValid())
		QDialog::accept();
	else
		QMessageBox::warning(this, windowTitle(), "You must choose one of the values.");
}
