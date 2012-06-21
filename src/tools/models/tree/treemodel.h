#ifndef TREEMODEL_H_
#define TREEMODEL_H_

#include <QtCore/QAbstractItemModel>
#include "items/treemodelitem.h"


MODELS_TREE_NS_BEGIN

/*
 * Notice:
 * 	Any QModelIndex instance must have internalPointer() == some "Item" instance!
 *
 */

class Model : public QAbstractItemModel
{
public:
    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

protected:
	class Container
	{
	public:
		typedef Tools::Models::Tree::Item Item;
		typedef Item::size_type           size_type;
		enum { InvalidIndex = Item::InvalidIndex };

	public:
		virtual ~Container();

		virtual size_type size() const = 0;
		virtual Item *at(size_type index) const = 0;
		virtual size_type indexOf(Item *item) const = 0;
	};

protected:
	Model(const Container &conteiner, QObject *parent = 0);

protected:
	QModelIndex index(Item *item) const;
	QModelIndex parent(Item *item) const;
	QModelIndex parent(Item *item, Item::size_type &row) const;

private:
	const Container &m_conteiner;
};

MODELS_TREE_NS_END

#endif /* TREEMODEL_H_ */
