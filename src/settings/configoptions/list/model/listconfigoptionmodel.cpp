#include "listconfigoptionmodel.h"


ListConfigOptionModel::ListConfigOptionModel(const QVariant &defaultValue, QObject *parent) :
	QAbstractItemModel(parent),
	m_items(defaultValue.toStringList())
{}

int ListConfigOptionModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
    	return m_items.size();
}

int ListConfigOptionModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant ListConfigOptionModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
		return m_items.at(index.row());
	else
		return QVariant();
}

bool ListConfigOptionModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	m_items[index.row()] = value.toString();
	return true;
}

Qt::ItemFlags ListConfigOptionModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant ListConfigOptionModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex ListConfigOptionModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		return createIndex(row, column);
    else
        return QModelIndex();
}

QModelIndex ListConfigOptionModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

QVariant ListConfigOptionModel::value() const
{
	return m_items;
}

void ListConfigOptionModel::setValue(const QVariant &value)
{
	beginResetModel();
	m_items = value.toStringList();
	endResetModel();
}

void ListConfigOptionModel::add(const QString &value)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	m_items.push_back(value);
	endInsertRows();
}

void ListConfigOptionModel::remove(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	m_items.removeAt(index.row());
	endInsertRows();
}
