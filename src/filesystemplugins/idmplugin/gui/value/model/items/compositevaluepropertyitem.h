#ifndef COMPOSITEVALUEPROPERTYITEM_H_
#define COMPOSITEVALUEPROPERTYITEM_H_

#include "../../../../model/items/idmlistitem.h"
#include "../../../../storage/entities/idmentity.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValuePropertyItem : public IdmListItem
{
public:
	CompositeValuePropertyItem(const IdmEntity::Property &property, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	IdmEntity *entity() const { return m_property.entity; }

protected:
	friend class CompositeValueModel;
	void add(IdmItem *item) { m_items.push_back(item); }
	void remove(size_type index) { delete m_items.takeAt(index); }

private:
	const IdmEntity::Property &m_property;
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEPROPERTYITEM_H_ */
