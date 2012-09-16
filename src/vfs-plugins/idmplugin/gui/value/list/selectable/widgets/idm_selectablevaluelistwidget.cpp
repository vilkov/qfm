#include "idm_selectablevaluelistwidget.h"


SelectableValueListWidget::SelectableValueListWidget(const IdmContainer &container, const Select &query, NestedDialog *parent) :
	EditableValueListWidget(container, query, parent)
{}

void SelectableValueListWidget::accept()
{
	if (currentIndex().isValid())
		EditableValueListWidget::accept();
	else
		warning(tr("You must choose one of the values."));
}
