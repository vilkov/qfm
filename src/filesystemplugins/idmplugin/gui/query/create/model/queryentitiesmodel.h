#ifndef QUERYENTITIESMODEL_H_
#define QUERYENTITIESMODEL_H_

#include "../../../../model/idmmodel.h"
#include "../../../../storage/entities/idmentity.h"


IDM_PLUGIN_NS_BEGIN

class QueryEntitiesModel : public IdmModel
{
public:
	typedef Container::size_type size_type;

public:
	QueryEntitiesModel(QObject *parent = 0);

	void add(const QString &name, IdmEntity *entity);
	void remove(const QModelIndex &index);

private:
	void expand(IdmItem *parent);
};

IDM_PLUGIN_NS_END

#endif /* QUERYENTITIESMODEL_H_ */
