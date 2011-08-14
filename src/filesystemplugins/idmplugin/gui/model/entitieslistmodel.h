#ifndef ENTITIESLISTMODEL_H_
#define ENTITIESLISTMODEL_H_

#include <QtCore/QAbstractItemModel>
#include "../../items/idmitemslist.h"
#include "../../../../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

class EntitiesListModel : public QAbstractItemModel
{
public:
	EntitiesListModel(const IdmItemsList *exsisting, QObject *parent = 0);

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

private:
	const IdmItemsList *m_exsisting;
	PScopedPointer<IdmItemsList> m_items;
};

FILE_SYSTEM_NS_END

#endif /* ENTITIESLISTMODEL_H_ */
