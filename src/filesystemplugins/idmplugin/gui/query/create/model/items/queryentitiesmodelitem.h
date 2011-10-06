#ifndef QUERYENTITIESMODELITEM_H_
#define QUERYENTITIESMODELITEM_H_

#include "../../../../../items/idmentityitem.h"


IDM_PLUGIN_NS_BEGIN

class QueryEntitiesModelItem : public IdmEntityItem
{
public:
	QueryEntitiesModelItem(const QString &name, IdmEntity *entity, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

protected:
	friend class QueryEntitiesModel;
	void add(IdmEntityItem *item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }

private:
	QVariant m_name;
};

IDM_PLUGIN_NS_END

#endif /* QUERYENTITIESMODELITEM_H_ */
