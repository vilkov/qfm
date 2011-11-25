#include "newfilevaluedialog.h"
#include "../../model/items/compositevaluepropertyitem.h"


NewFileValueDialog::NewFileValueDialog(const IdmContainer &container, IdmCompositeEntityValue *value, QWidget *parent) :
	NewCompositeValueDialog(container, value, parent)
{}

void NewFileValueDialog::addValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid() && static_cast<IdmItem*>(index.internalPointer())->isList() &&
		static_cast<CompositeValuePropertyItem*>(index.internalPointer())->entity()->type() != Database::Path)
		doAddValue(index);
}

void NewFileValueDialog::removeValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid() && !static_cast<IdmItem*>(index.internalPointer())->isList() &&
		static_cast<CompositeValuePropertyItem*>(index.internalPointer())->entity()->type() != Database::Path)
		doRemoveValue(index);
}
