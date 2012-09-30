#include "idm_editcompositevaluedialog.h"


EditCompositeValueDialog::EditCompositeValueDialog(const IdmContainer &container, const EntityValue::Holder &value, QWidget *parent) :
	CompositeValueDialog(container, value, parent)
{
	setWindowTitle(tr("Editing value of \"%1\"").arg(value->entity()->name()));
}
