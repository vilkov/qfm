#ifndef ENTITIESTREEMODEL_H_
#define ENTITIESTREEMODEL_H_

#include "../../../model/idmmodel.h"
#include "../../../storage/entities/idmentity.h"


IDM_PLUGIN_NS_BEGIN

class EntitiesTreeModel : public IdmModel
{
public:
	EntitiesTreeModel(QObject *parent = 0);

	void add(IdmEntity *entity);
	void remove(const QModelIndex &index);

private:
	void doAdd(IdmEntity *entity);
	void doAdd(IdmItem *item, IdmEntity *property);
	void doRemove(IdmItem *item, Container::size_type index);
	void expand(IdmItem *parent);

private:
	typedef QMap<IdmEntity*, Container> Map;
	Map m_entities;
};

IDM_PLUGIN_NS_END

#endif /* ENTITIESTREEMODEL_H_ */
