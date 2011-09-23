#ifndef QUERYENTITIESMODEL_H_
#define QUERYENTITIESMODEL_H_

#include <QtCore/QAbstractItemModel>
#include "items/queryentitiesmodelitemroot.h"


IDM_PLUGIN_NS_BEGIN

class QueryEntitiesModel : public QAbstractItemModel
{
public:
	typedef QueryEntitiesModelItemRoot::size_type size_type;

public:
	QueryEntitiesModel(QObject *parent = 0);
	virtual ~QueryEntitiesModel();

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	void add(const QString &name, IdmEntity *entity);
	void remove(const QModelIndex &index);

private:
	QueryEntitiesModelItemRoot m_items;
};

IDM_PLUGIN_NS_END

#endif /* QUERYENTITIESMODEL_H_ */
