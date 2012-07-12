#include "editcompositevaluedialog.h"


EditCompositeValueDialog::EditCompositeValueDialog(const IdmContainer &container, const IdmEntityValue::Holder &value, QWidget *parent) :
	CompositeValueDialog(container, value, parent)
{
	setWindowTitle(tr("Editing value of \"%1\"").arg(value->entity()->name()));
}
