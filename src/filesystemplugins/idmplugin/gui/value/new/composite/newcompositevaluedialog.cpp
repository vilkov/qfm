#include "newcompositevaluedialog.h"


NewCompositeValueDialog::NewCompositeValueDialog(const IdmContainer &container, const IdmEntityValue::Holder &value, QWidget *parent) :
	CompositeValueDialog(container, value, parent)
{
	setWindowTitle(tr("New value for \"%1\"").arg(value->entity()->name()));
}
