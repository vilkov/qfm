#ifndef IDM_STORAGE_UNDOREMOVEENTITY_H_
#define IDM_STORAGE_UNDOREMOVEENTITY_H_

#include <QtCore/QList>
#include "../idm_storage_undocommand.h"


IDM_PLUGIN_NS_BEGIN

class StorageUndoRemoveEntity : public StorageUndoCommand
{
public:
	typedef QList<Entity::Property> Parents;

public:
	StorageUndoRemoveEntity(Entity *entity);
	virtual ~StorageUndoRemoveEntity();

	virtual void undo(RootEntity &root);

	void addParent(Entity *entity, const QString &name) { m_parents.push_back(Entity::Property(entity, name)); }

private:
	Entity *m_entity;
	Parents m_parents;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOREMOVEENTITY_H_ */
