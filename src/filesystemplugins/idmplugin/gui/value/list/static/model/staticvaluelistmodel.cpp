#include "staticvaluelistmodel.h"


IDM_PLUGIN_NS_BEGIN

StaticValueListModel::StaticValueListModel(const IdmContainer &container, const Select &query, QObject *parent) :
	QAbstractItemModel(parent),
	m_reader(container, query)
{}

StaticValueListModel::~StaticValueListModel()
{
	qDeleteAll(m_items);
}

int StaticValueListModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
		return m_items.size();
}

int StaticValueListModel::columnCount(const QModelIndex &parent) const
{
	return 1;
}

QVariant StaticValueListModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
		switch (index.column())
		{
			case 0: return m_items.at(index.row())->value();
		}

	return QVariant();
}

Qt::ItemFlags StaticValueListModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant StaticValueListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

void StaticValueListModel::fetchMore(const QModelIndex &parent)
{
	List list;
	IdmEntityValue *item;

	list.reserve(PrefetchLimit);

	for (qint32 actualLimit = 0; actualLimit < PrefetchLimit; ++actualLimit)
		if (item = m_reader.next())
			list.push_back(item);
		else
			break;

	if (!list.isEmpty())
		add(list);
}

bool StaticValueListModel::canFetchMore(const QModelIndex &parent) const
{
    return !parent.isValid() && !m_reader.eof();
}

QModelIndex StaticValueListModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		return createIndex(row, column, m_items.at(row));
    else
        return QModelIndex();
}

QModelIndex StaticValueListModel::parent(const QModelIndex &child) const
{
    return QModelIndex();
}

void StaticValueListModel::add(const List &list)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + list.size() - 1);
	m_items.append(list);
	endInsertRows();
}

void StaticValueListModel::add(IdmEntityValue *value)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	m_items.push_back(value);
	endInsertRows();
}

void StaticValueListModel::add(Database::id_type id, const QVariant &value)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	m_items.push_back(IdmValueReader::createValue(m_reader.entity(), id, value));
	endInsertRows();
}

void StaticValueListModel::remove(const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), index.row(), index.row());
	delete m_items.takeAt(index.row());
	endRemoveRows();
}

IdmEntityValue *StaticValueListModel::take(const QModelIndex &index)
{
	IdmEntityValue *res;

	beginRemoveRows(QModelIndex(), index.row(), index.row());
	res = m_items.takeAt(index.row());
	endRemoveRows();

	return res;
}

IDM_PLUGIN_NS_END
