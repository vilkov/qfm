#ifndef IDMSTORAGEUNDOREMOVEENTITY_H_
#define IDMSTORAGEUNDOREMOVEENTITY_H_

#include <QtCore/QList>
#include "../idmstorageundocommand.h"


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

#endif /* IDMSTORAGEUNDOREMOVEENTITY_H_ */
