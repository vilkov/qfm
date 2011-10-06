#ifndef ENTITIESLISTMODEL_H_
#define ENTITIESLISTMODEL_H_

#include "../../../model/idmmodel.h"
#include "../../../storage/entities/idmentity.h"


IDM_PLUGIN_NS_BEGIN

class EntitiesListModel : public IdmModel
{
public:
	typedef Container::size_type size_type;

public:
	EntitiesListModel(QObject *parent = 0);

    /* QAbstractItemModel */
	virtual int columnCount(const QModelIndex &parent) const;

	IdmEntity *entityAt(size_type index) const;
	QString nameAt(size_type index) const;
	size_type size() const { return m_items.size(); }

	void add(IdmEntity *entity, const QString &name);
	void remove(const QModelIndex &index);
};

IDM_PLUGIN_NS_END

#endif /* ENTITIESLISTMODEL_H_ */
