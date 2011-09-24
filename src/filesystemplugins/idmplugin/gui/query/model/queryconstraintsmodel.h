#ifndef QUERYCONSTRAINTSMODEL_H_
#define QUERYCONSTRAINTSMODEL_H_

#include <QtCore/QAbstractItemModel>
#include "items/queryconstraintsmodelitem.h"


IDM_PLUGIN_NS_BEGIN

class QueryConstraintsModel : public QAbstractItemModel
{
public:
	QueryConstraintsModel(QObject *parent = 0);
	virtual ~QueryConstraintsModel();

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	void add();
	void remove(const QModelIndex &index);

private:
	typedef QList<QueryConstraintsModelItem*> Container;

private:
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* QUERYCONSTRAINTSMODEL_H_ */
