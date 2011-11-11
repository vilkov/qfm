#ifndef COMPOSITEVALUEPROPERTYITEM_H_
#define COMPOSITEVALUEPROPERTYITEM_H_

#include "compositevalueitem.h"
#include "../../../../storage/entities/idmentity.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValuePropertyItem : public CompositeValueItem
{
public:
	CompositeValuePropertyItem(const IdmEntity::Property &property, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

protected:
	friend class CompositeValueModel;
	void add(CompositeValueItem *item) { m_items.push_back(item); }
	void remove(size_type index) { delete m_items.takeAt(index); }

private:
	const IdmEntity::Property &m_property;
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEPROPERTYITEM_H_ */
