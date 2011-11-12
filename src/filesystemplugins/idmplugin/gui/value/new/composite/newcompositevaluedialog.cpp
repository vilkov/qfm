#include "newcompositevaluedialog.h"
#include "../../model/items/compositevaluevalueitem.h"
#include "../../model/items/compositevaluepropertyitem.h"
#include "../../../../storage/values/idmvaluereader.h"
#include "../../../../../../tools/pointers/pscopedpointer.h"


NewCompositeValueDialog::NewCompositeValueDialog(const IdmContainer &container, IdmEntity *entity, QWidget *parent) :
	CompositeValueDialog(container, entity, parent),
	m_entity(entity)
{
	setWindowTitle(tr("New value for \"%1\"").arg(m_entity->name()));
}

IdmEntityValue *NewCompositeValueDialog::value()
{
	IdmContainer::id_type id = container().addValue(m_entity);

	if (id != IdmContainer::InvalidId)
	{
		IdmEntityValue *tmp;
		IdmContainer::IdsMap res;
		CompositeValuePropertyItem *property;
		PScopedPointer<IdmEntityValue> value(IdmValueReader::createValue(m_entity, id));

		for (CompositeValueModel::size_type i = 0, size = model().size(); i < size; ++i)
		{
			IdmContainer::IdsList &list = res[(property = static_cast<CompositeValuePropertyItem*>(model().at(i)))->entity()];
			list.reserve(property->size());

			for (CompositeValueModel::size_type q = 0, size = property->size(); q < size; ++q)
			{
				IdmValueReader::addValue(value.data(), tmp = static_cast<CompositeValueValueItem*>(property->at(q))->take());
				list.push_back(tmp->id());
			}
		}

		if (container().addValue(m_entity, id, res))
			return value.take();
	}

	return 0;
}
