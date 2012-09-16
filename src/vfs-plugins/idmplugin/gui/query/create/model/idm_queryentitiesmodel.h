#ifndef IDM_QUERYENTITIESMODEL_H_
#define IDM_QUERYENTITIESMODEL_H_

#include "../../../../model/idm_model.h"
#include "../../../../storage/entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class QueryEntitiesModel : public Model
{
public:
	typedef Container::size_type size_type;

public:
	QueryEntitiesModel(QObject *parent = 0);

	void add(const IdmEntity::Property &property);
	void remove(const QModelIndex &index);

private:
	void expand(IdmItem *parent);
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYENTITIESMODEL_H_ */
