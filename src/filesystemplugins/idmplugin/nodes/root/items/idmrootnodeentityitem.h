#ifndef IDMROOTNODEENTITYITEM_H_
#define IDMROOTNODEENTITYITEM_H_

#include "idmrootnodeentitybaseitem.h"


IDM_PLUGIN_NS_BEGIN

class RootNodeEntityItem : public RootNodeEntityBaseListItem
{
public:
	RootNodeEntityItem(IdmEntity *entity, Base *parent = 0);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	IdmEntity *entity() const { return m_entity; }

protected:
	friend class IdmRootNode;
	void add(Base *item) { m_items.push_back(item); }
	void remove(size_type index) { delete m_items.takeAt(index); }

private:
	IdmEntity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDMROOTNODEENTITYITEM_H_ */
