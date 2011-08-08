#ifndef MODELITEM_H_
#define MODELITEM_H_

#include <QObject>
#include <QList>
#include <QMap>
#include <QVariant>
#include <QModelIndex>


class ModelItem
{
	Q_DISABLE_COPY(ModelItem)

public:
	typedef QList<ModelItem*> value_type;
	typedef value_type::size_type size_type;

protected:
	ModelItem *parentItem;
	value_type childItems;

public:
	ModelItem(ModelItem *parent = 0) :
		parentItem(parent)
	{}

	virtual ~ModelItem()
	{}

	const QList<ModelItem*> &childs() const { return childItems; }
	ModelItem *child(const size_type index) const { return childItems.value(index); }
	size_type indexOf(ModelItem *item) const { return childItems.indexOf(item); }
	size_type size() const { return childItems.size(); }
	bool isEmpty() const { return childItems.isEmpty(); }

	virtual void appendChild(ModelItem *child)
	{
		childItems.append(child);
		child->parentItem = this;
	}

	void removeChild(ModelItem *child)
	{
		int index;

		if ((index = childItems.indexOf(child)) != -1)
			childItems.removeAt(index);
	}

	void removeChild(const int index)
	{
		childItems.removeAt(index);
	}

	inline void insertChild(ModelItem *baseItem, ModelItem *child)
	{
		if (baseItem)
			childItems.insert(childItems.indexOf(baseItem, 0), child);
		else
			childItems.insert(0, child);

		child->parentItem = this;
	}

	inline void changeIndex(ModelItem *newIndex, ModelItem *child)
	{
		childItems.removeAt(childItems.indexOf(child, 0));
		insertChild(newIndex, child);
	}

	inline void clear()
	{
		for (size_type i = 0; i < childItems.size(); ++i)
			delete childItems[i];

		childItems.clear();
	}

	inline int row() const
	{
		if (parentItem)
			return parentItem->childItems.indexOf(const_cast<ModelItem*>(this));

		return 0;
	}

	inline ModelItem *parent() const { return parentItem; }

	virtual int columnCount() const = 0;
	virtual QVariant data(int column, int role) const = 0;
};

#endif /* MODELITEM_H_ */
