#include "newfilevaluedialog.h"
#include "../../model/items/compositevaluepropertyitem.h"


NewFileValueDialog::NewFileValueDialog(const IdmContainer &container, const IdmEntityValue::Holder &value, const CompositeValueModel::Files &files, QWidget *parent) :
	NewCompositeValueDialog(container, value, files, parent)
{}

void NewFileValueDialog::addValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid() && static_cast<CompositeValueItem *>(index.internalPointer())->isProperty() &&
		static_cast<CompositeValuePropertyItem*>(index.internalPointer())->entity()->type() != Database::Path)
		doAddValue(index);
}

void NewFileValueDialog::removeValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid() && !static_cast<CompositeValueItem *>(index.internalPointer())->isProperty() &&
		static_cast<CompositeValuePropertyItem*>(index.internalPointer())->entity()->type() != Database::Path)
		doRemoveValue(index);
}
