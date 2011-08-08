#ifndef TASKMODEL_H_
#define TASKMODEL_H_

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include "items/modelitem.h"


class Model : public QAbstractItemModel
{
	Q_OBJECT
	Q_DISABLE_COPY(Model)

protected:
	ModelItem *rootItem;

public:
	Model(QObject *parent = 0);
	~Model();

	QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	QModelIndex index(int row, int column, ModelItem *parentItem) const;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	QModelIndex index(int column, ModelItem *item) const;
	QModelIndex parent(const QModelIndex &index) const;
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	void insertRows(ModelItem *parent, const QList<ModelItem*> &data);
	void appendRows(ModelItem *parent, const QList<ModelItem*> &data);
	void removeRows(ModelItem *parent, const bool isParent = true);
	void update(const QModelIndex &topLeft, const QModelIndex &bottomRight);
};

#endif /*TASKMODEL_H_*/
