#ifndef IDM_STORAGE_UNDOREMOVEENTITY_H_
#define IDM_STORAGE_UNDOREMOVEENTITY_H_

#include <QtCore/QList>
#include "../idm_storage_undocommand.h"


IDM_PLUGIN_NS_BEGIN

class IdmStorageUndoRemoveEntity : public IdmStorageUndoCommand
{
public:
	typedef QList<IdmEntity::Property> Parents;

public:
	IdmStorageUndoRemoveEntity(IdmEntity *entity);
	virtual ~IdmStorageUndoRemoveEntity();

	virtual void undo(IdmEntityRoot &root);

	void addParent(IdmEntity *entity, const QString &name) { m_parents.push_back(IdmEntity::Property(entity, name)); }

private:
	IdmEntity *m_entity;
	Parents m_parents;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOREMOVEENTITY_H_ */
