#ifndef COMPOSITEVALUEPROPERTYITEM_H_
#define COMPOSITEVALUEPROPERTYITEM_H_

#include "compositevalueitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValuePropertyItem : public CompositeValueItem
{
public:
	typedef QList<IdmItem *> Container;

public:
	CompositeValuePropertyItem(const IdmEntity::Property &property, IdmItem *parent = 0);
	virtual ~CompositeValuePropertyItem();

	/* Base */
	virtual Base *at(size_type index) const;
	virtual size_type size() const;
	virtual size_type indexOf(Base *item) const;

	virtual QVariant data(qint32 column, qint32 role) const;

	/* CompositeValueItem */
	virtual bool isProperty() const;

	IdmEntity *entity() const { return m_property.entity; }

protected:
	friend class CompositeValueModel;
	void add(IdmItem *item) { m_items.push_back(item); }
	void remove(size_type index) { delete m_items.takeAt(index); }

private:
	IdmEntity::Property m_property;
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEPROPERTYITEM_H_ */
