#ifndef IDM_ROOTNODEENTITYITEM_H_
#define IDM_ROOTNODEENTITYITEM_H_

#include "idm_rootnodeitem.h"
#include "../../../storage/entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class RootNodeEntityItem : public RootNodeListItem
{
public:
	RootNodeEntityItem(Entity *entity, Base *parent = 0);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* RootNodeItem */
	virtual bool isEntity();

	Entity *entity() const { return m_entity; }

protected:
	friend class RootNode;
	void add(RootNodeItem *item) { m_items.push_back(item); }
	void remove(size_type index) { delete m_items.takeAt(index); }

private:
	Entity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDM_ROOTNODEENTITYITEM_H_ */
