#ifndef VALUESTREEMODEL_H_
#define VALUESTREEMODEL_H_

#include <QtCore/QAbstractItemModel>
#include "items/valuesroottreeitem.h"


IDM_PLUGIN_NS_BEGIN

class ValuesTreeModel : public QAbstractItemModel
{
public:
	typedef ValuesTreeItem::size_type size_type;

public:
	ValuesTreeModel(IdmEntity *entity, QObject *parent = 0);
	virtual ~ValuesTreeModel();

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
	ValuesRootTreeItem m_items;
};

IDM_PLUGIN_NS_END

#endif /* VALUESTREEMODEL_H_ */
