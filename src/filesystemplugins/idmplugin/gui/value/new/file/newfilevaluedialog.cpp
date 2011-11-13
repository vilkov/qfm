#include "newfilevaluedialog.h"
#include "../../model/items/compositevaluepropertyitem.h"
#include <QtGui/QMessageBox>


NewFileValueDialog::NewFileValueDialog(const IdmContainer &container, IdmCompositeEntityValue *value, const QStringList &files, QWidget *parent) :
	NewCompositeValueDialog(container, value, parent)
{
	IdmEntity *entity = value->entity();

	for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
    	if (entity->at(i).entity->type() == Database::Path)
    	{
    		IdmEntityValue *value;
    		CompositeValueModel::ValueList list;
    		QModelIndex index = model().index(i, 0);
    		entity = entity->at(i).entity;

			for (QStringList::size_type i = 0, size = files.size(); i < size; ++i)
				if (value = container.addValue(entity, files.at(i)))
					list.push_back(value);
				else
				{
					qDeleteAll(list);
					list.clear();
					break;
				}

			if (list.isEmpty())
				QMessageBox::critical(this, windowTitle(), container.lastError());
			else
				model().add(index, list);

			break;
    	}
}

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
