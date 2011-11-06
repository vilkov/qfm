#include "newfilevaluedialog.h"
#include "../../../../items/idmentityitem.h"
#include "../../../../storage/values/idmvaluereader.h"
#include <QtGui/QMessageBox>


NewFileValueDialog::NewFileValueDialog(const IdmContainer &container, IdmEntity *entity, const QStringList &files, QWidget *parent) :
	NewCompositeValueDialog(container, entity, parent)
{
    for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
    	if (entity->at(i).entity->type() == Database::Path)
    	{
    		QVariant value;
    		IdmEntity::id_type id;
    		CompositeValueModel::ValueList list;
    		QModelIndex index = model().index(i, 0);
    		entity = entity->at(i).entity;

			for (QStringList::size_type i = 0, size = files.size(); i < size; ++i)
				if ((id = container.addValue(entity, value = files.at(i))) == IdmEntity::InvalidId)
				{
					qDeleteAll(list);
					list.clear();
					break;
				}
				else
					list.push_back(IdmValueReader::createValue(entity, id, value));

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

	if (index.isValid() &&
		static_cast<IdmEntityItem*>(index.internalPointer())->entity()->type() != Database::Path)
		doAddValue(index);
}

void NewFileValueDialog::removeValue()
{
	QModelIndex index = currentIndex();

	if (index.isValid() &&
		static_cast<IBaseItem*>(index.internalPointer())->isValueItem() &&
		static_cast<IdmEntityItem*>(index.internalPointer())->entity()->type() != Database::Path)
		doRemoveValue(index);
}
