#include "idm_newcompositevaluedialog.h"


NewCompositeValueDialog::NewCompositeValueDialog(const IdmContainer &container, const EntityValue::Holder &value, QWidget *parent) :
	CompositeValueDialog(container, value, parent)
{
	setWindowTitle(tr("New value for \"%1\"").arg(value->entity()->name()));
}

NewCompositeValueDialog::NewCompositeValueDialog(const IdmContainer &container, const EntityValue::Holder &value, const CompositeValueModel::Files &files, QWidget *parent) :
	CompositeValueDialog(container, value, files, parent)
{
	setWindowTitle(tr("New value for \"%1\"").arg(value->entity()->name()));
}
