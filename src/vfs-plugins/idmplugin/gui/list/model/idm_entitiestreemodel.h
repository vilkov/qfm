#ifndef IDM_ENTITIESTREEMODEL_H_
#define IDM_ENTITIESTREEMODEL_H_

#include "../../../model/idm_model.h"
#include "../../../storage/entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class EntitiesTreeModel : public Model
{
public:
	EntitiesTreeModel(QObject *parent = 0);

	void add(Entity *entity);
	void remove(const QModelIndex &index);

private:
	void doAdd(Entity *entity);
	void doAdd(IdmItem *item, Entity *property);
	void doRemove(IdmItem *item, Container::size_type index);
	void expand(IdmItem *parent);

private:
	typedef QMap<Entity*, Container::List> Map;
	Map m_entities;
};

IDM_PLUGIN_NS_END

#endif /* IDM_ENTITIESTREEMODEL_H_ */
