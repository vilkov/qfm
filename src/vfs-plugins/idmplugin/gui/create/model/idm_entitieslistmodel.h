#ifndef IDM_ENTITIESLISTMODEL_H_
#define IDM_ENTITIESLISTMODEL_H_

#include "../../../model/idm_model.h"
#include "../../../storage/entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class EntitiesListModel : public Model
{
public:
	typedef Container::size_type size_type;

public:
	EntitiesListModel(QObject *parent = 0);

    /* Model */
	virtual int columnCount(const QModelIndex &parent) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;

	Entity *entityAt(size_type index) const;
	const QString &nameAt(size_type index) const;
	size_type size() const { return m_items.size(); }

	void add(Entity *entity, const QString &name);
	void remove(const QModelIndex &index);
};

IDM_PLUGIN_NS_END

#endif /* IDM_ENTITIESLISTMODEL_H_ */
