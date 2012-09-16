#ifndef IDM_MENUENTITIESTREEITEM_H_
#define IDM_MENUENTITIESTREEITEM_H_

#include "../../../../model/items/idm_listitem.h"
#include "../../../../storage/entities/idm_entity.h"


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

#endif /* IDM_MENUENTITIESTREEITEM_H_ */
