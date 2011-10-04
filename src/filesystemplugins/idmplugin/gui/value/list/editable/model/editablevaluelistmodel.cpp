#include "editablevaluelistmodel.h"


IDM_PLUGIN_NS_BEGIN

EditableValueListModel::EditableValueListModel(const IdmContainer &container, const Select &query, QObject *parent) :
	QAbstractItemModel(parent),
	m_reader(container, query)
{
	if (m_reader.isValid())
	{
		List list;
		IdmEntityValue *item;

		while (item = m_reader.next())
			list.push_back(item);

		if (!list.isEmpty())
			add(list);
	}
}

EditableValueListModel::~EditableValueListModel()
{
	qDeleteAll(m_items);
}

int EditableValueListModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return m_items.size();
}

int EditableValueListModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant EditableValueListModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
		switch (index.column())
		{
			case 0: return m_items.at(index.row())->value();
		}

	return QVariant();
}

Qt::ItemFlags EditableValueListModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant EditableValueListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex EditableValueListModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		return createIndex(row, column, m_items.at(row));
    else
        return QModelIndex();
}

QModelIndex EditableValueListModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

void EditableValueListModel::add(const List &list)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + list.size() - 1);
	m_items.append(list);
	endInsertRows();
}

void EditableValueListModel::add(IdmEntityValue *value)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	m_items.push_back(value);
	endInsertRows();
}

void EditableValueListModel::add(Database::id_type id, const QVariant &value)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	m_items.push_back(IdmValueReader::createValue(m_reader.entity(), id, value));
	endInsertRows();
}

void EditableValueListModel::remove(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	delete m_items.takeAt(index.row());
	endRemoveRows();
}

IdmEntityValue *EditableValueListModel::take(const QModelIndex &index)
{
	IdmEntityValue *res;

	beginRemoveRows(QModelIndex(), index.row(), index.row());
	res = m_items.takeAt(index.row());
	endRemoveRows();

	return res;
}

IDM_PLUGIN_NS_END
