#include "valuelistmodel.h"
#include "../../../../storage/values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

ValueListModel::ValueListModel(const QueryContext &context, QObject *parent) :
	QAbstractItemModel(parent),
	m_context(context)
{
	if (m_context.isValid())
	{
		List list;

		switch (m_context.entity()->type())
		{
			case Database::Int:
			{
				while (m_context.next())
					list.push_back(new ValueListItem(m_context.asInt(0), contextValue<Database::Int>(m_context, 1)));

				break;
			}
			case Database::String:
			{
				while (m_context.next())
					list.push_back(new ValueListItem(m_context.asInt(0), contextValue<Database::String>(m_context, 1)));

				break;
			}
			case Database::Date:
			{
				while (m_context.next())
					list.push_back(new ValueListItem(m_context.asInt(0), contextValue<Database::Date>(m_context, 1)));

				break;
			}
			case Database::Time:
			{
				while (m_context.next())
					list.push_back(new ValueListItem(m_context.asInt(0), contextValue<Database::Time>(m_context, 1)));

				break;
			}
			case Database::DateTime:
			{
				while (m_context.next())
					list.push_back(new ValueListItem(m_context.asInt(0), contextValue<Database::DateTime>(m_context, 1)));

				break;
			}
			case Database::Memo:
			{
				while (m_context.next())
					list.push_back(new ValueListItem(m_context.asInt(0), contextValue<Database::Memo>(m_context, 1)));

				break;
			}
			case Database::Rating:
			{
				while (m_context.next())
					list.push_back(new ValueListItem(m_context.asInt(0), contextValue<Database::Rating>(m_context, 1)));

				break;
			}
			case Database::Path:
			{
				while (m_context.next())
					list.push_back(new ValueListItem(m_context.asInt(0), contextValue<Database::Path>(m_context, 1)));

				break;
			}
			default:
				break;
		}

		if (!list.isEmpty())
			add(list);
	}
}

ValueListModel::~ValueListModel()
{
	qDeleteAll(m_items);
}

int ValueListModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;
	else
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
