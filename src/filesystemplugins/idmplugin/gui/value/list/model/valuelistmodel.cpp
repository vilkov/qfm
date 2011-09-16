#include "valuelistmodel.h"


IDM_PLUGIN_NS_BEGIN

ValueListModel::ValueListModel(QObject *parent) :
	QAbstractItemModel(parent)
{}

ValueListModel::~ValueListModel()
{
	qDeleteAll(m_items);
}

int ValueListModel::rowCount(const QModelIndex &parent) const
{
   	return m_items.size();
}

int ValueListModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant ValueListModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
		switch (index.column())
		{
			case 0: return m_items.at(index.row())->value();
		}

	return QVariant();
}

Qt::ItemFlags ValueListModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant ValueListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex ValueListModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		return createIndex(row, column, m_items.at(row));
    else
        return QModelIndex();
}

QModelIndex ValueListModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

void ValueListModel::add(const List &list)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + list.size() - 1);
	m_items.append(list);
	endInsertRows();
}

void ValueListModel::add(Database::id_type id, const QVariant &value)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	m_items.push_back(new ValueListItem(id, value));
	endInsertRows();
}

void ValueListModel::remove(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	delete m_items.takeAt(index.row());
	endRemoveRows();
}

IDM_PLUGIN_NS_END
