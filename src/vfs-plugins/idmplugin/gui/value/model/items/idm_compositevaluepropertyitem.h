#ifndef IDM_COMPOSITEVALUEPROPERTYITEM_H_
#define IDM_COMPOSITEVALUEPROPERTYITEM_H_

#include "idm_compositevalueitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValuePropertyItem : public CompositeValueItem
{
public:
	typedef QList<IdmItem *> Container;

public:
	CompositeValuePropertyItem(const Entity::Property &property, IdmItem *parent = 0);
	virtual ~CompositeValuePropertyItem();

	/* Base */
	virtual Base *at(size_type index) const;
	virtual size_type size() const;
	virtual size_type indexOf(Base *item) const;

	virtual QVariant data(qint32 column, qint32 role) const;

	/* CompositeValueItem */
	virtual bool isProperty() const;

	Entity *entity() const { return m_property.entity; }

protected:
	friend class CompositeValueModel;
	void add(IdmItem *item) { m_items.push_back(item); }
	void remove(size_type index) { delete m_items.takeAt(index); }

private:
	Entity::Property m_property;
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEPROPERTYITEM_H_ */
