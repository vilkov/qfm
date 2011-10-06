#ifndef QUERYENTITIESMODELITEM_H_
#define QUERYENTITIESMODELITEM_H_

#include "../../../../../items/idmentitypropertyitem.h"


IDM_PLUGIN_NS_BEGIN

class QueryEntitiesModelItem : public IdmEntityPropertyItem
{
public:
	QueryEntitiesModelItem(const QString &name, IdmEntity *entity, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

protected:
	friend class QueryEntitiesModel;
	void add(IdmEntityPropertyItem *item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }
};

IDM_PLUGIN_NS_END

#endif /* QUERYENTITIESMODELITEM_H_ */
