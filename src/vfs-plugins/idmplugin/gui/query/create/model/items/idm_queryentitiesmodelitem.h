#ifndef IDM_QUERYENTITIESMODELITEM_H_
#define IDM_QUERYENTITIESMODELITEM_H_

#include "../../../../../model/items/idm_listitem.h"
#include "../../../../../storage/entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class QueryEntitiesModelItem : public IdmListItem
{
public:
	QueryEntitiesModelItem(const IdmEntity::Property &property, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	IdmEntity *entity() const { return m_property.entity; }
	const IdmEntity::Property &property() const { return m_property; }

protected:
	friend class QueryEntitiesModel;
	void add(IdmListItem *item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }

private:
	IdmEntity::Property m_property;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYENTITIESMODELITEM_H_ */
