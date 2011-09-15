#ifndef VALUELISTMODEL_H_
#define VALUELISTMODEL_H_

#include <QtCore/QAbstractItemModel>
#include "items/valuelistrootitem.h"
#include "../../../../storage/queries/idmquerycontext.h"


IDM_PLUGIN_NS_BEGIN

class ValueListModel : public QAbstractItemModel
{
public:
	typedef ValueListItem::size_type size_type;

public:
	ValueListModel(const QueryContext &context, QObject *parent = 0);

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	bool isValid() const { return m_context.isValid(); }
	IdmItem *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(IdmItem *item) const { return m_items.indexOf(item); }

	void add(IdmEntity *entity);
	void add(const QModelIndex &index, const QVariant &value);
	void remove(const QModelIndex &index);

private:
	QueryContext m_context;
	ValueListRootItem m_items;
};

IDM_PLUGIN_NS_END

#endif /* VALUELISTMODEL_H_ */
