#ifndef IDMMODEL_H_
#define IDMMODEL_H_

#include <QtCore/QAbstractItemModel>
#include "items/idmitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmModel : public QAbstractItemModel
{
public:
	IdmModel(QObject *parent = 0);
	virtual ~IdmModel();

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

protected:
	typedef QList<IdmItem*> Container;
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDMMODEL_H_ */
