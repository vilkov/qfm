#ifndef TREEMODEL_H_
#define TREEMODEL_H_

#include <QtCore/QAbstractItemModel>
#include "items/treeitem.h"


MODELS_NS_BEGIN

class TreeModel : public QAbstractItemModel
{
public:
	TreeModel(QObject *parent = 0);
	virtual ~TreeModel();

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

protected:
	typedef QList<TreeItem*> Container;
	Container m_items;
};

MODELS_NS_END

#endif /* TREEMODEL_H_ */
