#ifndef COMPOSITEVALUEMODEL_H_
#define COMPOSITEVALUEMODEL_H_

#include <QtCore/QAbstractItemModel>
#include "items/valuesroottreeitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueModel : public QAbstractItemModel
{
public:
	typedef ValuesRootTreeItem::List      List;
	typedef ValuesRootTreeItem::Map       Map;
	typedef ValuesRootTreeItem::ValueList ValueList;
	typedef ValuesRootTreeItem::size_type size_type;

public:
	CompositeValueModel(IdmEntity *entity, QObject *parent = 0);

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	const Map &entities() const { return m_items.entities(); }

	void add(const QModelIndex &index, IdmEntityValue *value);
	void add(const QModelIndex &index, const ValueList &values);
	void remove(const QModelIndex &index);

private:
	ValuesRootTreeItem m_items;
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEMODEL_H_ */
