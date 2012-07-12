#ifndef IDMMENUENTITIESTREEITEM_H_
#define IDMMENUENTITIESTREEITEM_H_

#include "../../../../model/items/idmlistitem.h"
#include "../../../../storage/entities/idmentity.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntitiesTreeItem : public IdmListItem
{
public:
	IdmEntitiesTreeItem(IdmEntity *entity, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	IdmEntity *entity() const { return m_entity; }

protected:
	friend class EntitiesTreeModel;
	void add(IdmListItem *item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }

private:
	IdmEntity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDMMENUENTITIESTREEITEM_H_ */
