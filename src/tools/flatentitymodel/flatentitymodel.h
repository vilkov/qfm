#ifndef FLATENTITYMODEL_H_
#define FLATENTITYMODEL_H_

#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QModelIndex>
#include <QtCore/QModelIndexList>
#include <QtCore/QAbstractItemModel>
#include "items/entityitem.h"
#include "../templates/metatemplates.h"


using namespace ::Tools;

class FlatEntityModel : public QAbstractItemModel
{
	Q_OBJECT
	Q_DISABLE_COPY(FlatEntityModel)

public:
	typedef QList<EntityItem*>    value_type;
	typedef value_type::size_type size_type;

public:
	FlatEntityModel(QObject *parent = 0);
	virtual ~FlatEntityModel();

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;
	size_type size() const { return m_entity.size(); }
	bool isEmpty() const { return m_entity.isEmpty(); }

	template <typename T> QList<qint32> ids() const
	{
		QList<qint32> res;

		for (size_type i = 0, size = m_entity.size(); i < size; ++i)
			res.push_back(static_cast<T*>(m_entity.at(i))->item().id());

		return res;
	}
	template <typename T> QMap<qint32, QModelIndex> idsAt(const QModelIndexList &list) const
	{
		QMap<qint32, QModelIndex> res;
		QSet<T*> items;

		for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
			if (!items.contains(static_cast<T*>(list.at(i).internalPointer())))
			{
				items.insert(static_cast<T*>(list.at(i).internalPointer()));
				res[static_cast<T*>(list.at(i).internalPointer())->item().id()] = list.at(i);
			}

		return res;
	}
	template <typename I, typename T> QList<I> items() const
	{
		QList<I> res;
		QSet<T*> items;

		for (size_type i = 0, size = m_entity.size(); i < size; ++i)
			if (!items.contains(static_cast<T*>(m_entity.at(i))))
			{
				items.insert(static_cast<T*>(m_entity.at(i)));
				res.push_back(static_cast<T*>(m_entity.at(i))->item());
			}

		return res;
	}
	template <typename I, typename T> QList<I> itemsAt(const QModelIndexList &list) const
	{
		QList<I> res;
		QSet<T*> items;

		for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
			if (!items.contains(static_cast<T*>(list.at(i).internalPointer())))
			{
				items.insert(static_cast<T*>(list.at(i).internalPointer()));
				res.push_back(static_cast<T*>(list.at(i).internalPointer())->item());
			}

		return res;
	}

	template <typename T, typename S> void add(const S &value)
	{
		using namespace Templates;
		must_have_base<T, EntityItem>();
		addItem<T, S>(value, int_to_type<has_value_type<S>::value>());
	}
	template <typename S> void append(const S &value)
	{
		using namespace Templates;
		appendItem<S>(value, int_to_type<has_value_type<S>::value>());
	}
	template <typename T> QList<T*> take(const QModelIndexList &items)
	{
		QSet<EntityItem*> done;
		EntityItem *item;
		QList<T*> res;
		qint32 index;

		for (QModelIndexList::size_type i = 0, size = items.size(); i < size; ++i)
			if (!done.contains(item = static_cast<EntityItem*>(items.at(i).internalPointer())))
			{
				done.insert(item);

				if (item->parent() == 0)
				{
					index = m_entity.indexOf(item);
					beginRemoveRows(QModelIndex(), index, index);
					res.push_back(static_cast<T*>(m_entity.takeAt(index)));
					endRemoveRows();
				}
			}

		return res;
	}

	void clear();
	void remove(const QModelIndex &item);
	void remove(const QModelIndexList &items);

protected:
	template <typename T, typename S> void addItem(const S &items, Templates::int_to_type<1>)
	{
		beginInsertRows(QModelIndex(), m_entity.size(), m_entity.size() + items.size() - 1);
		for (typename S::size_type i = 0, size = items.size(); i < size; ++i)
			m_entity.push_back(new T(items.at(i)));
		endInsertRows();
	}
	template <typename T, typename S> void addItem(const S &item, Templates::int_to_type<0>)
	{
		beginInsertRows(QModelIndex(), m_entity.size(), m_entity.size());
		m_entity.push_back(new T(item));
		endInsertRows();
	}
	template <typename S> void appendItem(const S &items, Templates::int_to_type<1>)
	{
		using namespace Templates;
		must_have_base<typename S::value_type, EntityItem>();
		beginInsertRows(QModelIndex(), m_entity.size(), m_entity.size() + items.size() - 1);
		for (typename S::size_type i = 0, size = items.size(); i < size; ++i)
			m_entity.push_back(items.at(i));
		endInsertRows();
	}
	template <typename S> void appendItem(const S &item, Templates::int_to_type<0>)
	{
		using namespace Templates;
		must_have_base<S, EntityItem>();
		beginInsertRows(QModelIndex(), m_entity.size(), m_entity.size());
		m_entity.push_back(item);
		endInsertRows();
	}

	QModelIndex index(int column, EntityItem *item) const;
	QModelIndex index(int row, int column, EntityItem *parentItem) const;

protected:
	value_type m_entity;
};

#endif /* FLATENTITYMODEL_H_ */
