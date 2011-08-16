#ifndef ENTITIESLISTMODEL_H_
#define ENTITIESLISTMODEL_H_

#include <QtCore/QAbstractItemModel>
#include "../../items/idmitem.h"
#include "../../storage/entities/idmentity.h"


FILE_SYSTEM_NS_BEGIN

class EntitiesListModel : public QAbstractItemModel
{
public:
	typedef QList<IdmItem*>       value_type;
	typedef value_type::size_type size_type;

public:
	EntitiesListModel(QObject *parent = 0);
	virtual ~EntitiesListModel();

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	void add(IdmEntity *entity);
	void remove(const QModelIndex &index);

private:
	value_type m_items;
};

FILE_SYSTEM_NS_END

#endif /* ENTITIESLISTMODEL_H_ */
