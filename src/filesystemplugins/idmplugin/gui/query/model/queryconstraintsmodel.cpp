#include "queryconstraintsmodel.h"
#include "../../../storage/constraints/value/idmidconstraint.h"


IDM_PLUGIN_NS_BEGIN

QueryConstraintsModel::QueryConstraintsModel(QObject *parent) :
	QAbstractItemModel(parent),
	m_andType(tr("AND")),
	m_orType(tr("OR")),
	m_andTypeLabel(tr("(...AND...)")),
	m_orTypeLabel(tr("(...OR...)"))
{}

int QueryConstraintsModel::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		if (static_cast<BaseConstraint*>(parent.internalPointer())->isGroup())
			return static_cast<GroupConstraint*>(parent.internalPointer())->size();
		else
			return 0;
	else
    	return 1;
}

int QueryConstraintsModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant QueryConstraintsModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return data(static_cast<BaseConstraint*>(index.internalPointer()), index.column(), role);
    else
    	return data(const_cast<QueryConstraintsRootItem*>(&m_root), index.column(), role);
}

Qt::ItemFlags QueryConstraintsModel::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant QueryConstraintsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex QueryConstraintsModel::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		if (parent.isValid() && static_cast<BaseConstraint*>(parent.internalPointer())->isGroup())
			return createIndex(row, column, static_cast<GroupConstraint*>(parent.internalPointer())->at(row));
		else
			return createIndex(row, column, const_cast<QueryConstraintsRootItem*>(&m_root));
    else
        return QModelIndex();
}

QModelIndex QueryConstraintsModel::parent(const QModelIndex &child) const
{
    if (child.isValid())
		if (BaseConstraint *parent = static_cast<BaseConstraint*>(child.internalPointer())->parent())
			if (parent->parent())
				return createIndex(static_cast<GroupConstraint*>(parent->parent())->indexOf(parent), 0, parent);
			else
				return createIndex(m_root.indexOf(parent), 0, parent);

    return QModelIndex();
}

void QueryConstraintsModel::add(const QModelIndex &index)
{
//	if (index.isValid() && static_cast<IdmItem*>(index.internalPointer())->isList())
//	{
//		beginInsertRows(index, static_cast<IdmListItem*>(index.internalPointer())->size(), static_cast<IdmListItem*>(index.internalPointer())->size());
//		m_items.push_back(new QueryGroupConstraintsItem());
//		endInsertRows();
//	}
}

void QueryConstraintsModel::remove(const QModelIndex &index)
{
//	beginRemoveRows(QModelIndex(), index.row(), index.row());
//	delete m_items.at(index.row());
//	m_items.remove(index.row());
//	endRemoveRows();
}

QVariant QueryConstraintsModel::data(BaseConstraint *item, int column, int role) const
{
	switch (column)
	{
		case 0:
		{
			if (role == Qt::DisplayRole)
				if (item->isGroup())
					switch (static_cast<GroupConstraint*>(item)->type())
					{
						case GroupConstraint::And:
							return m_andTypeLabel;

						case GroupConstraint::Or:
							return m_orTypeLabel;
					}
				else
					return QString::fromLatin1("%1 %2 %3").
							arg(static_cast<Constraint*>(item)->property().name).
							arg(Constraint::operatorToString(static_cast<ValueConstraint*>(item)->op())).
							arg(static_cast<ValueConstraint*>(item)->value().toString());

			break;
		}

		case 1:
		{
			if (item->isGroup())
				switch (role)
				{
					case Qt::DisplayRole:
					{
						switch (static_cast<GroupConstraint*>(item)->type())
						{
							case GroupConstraint::And:
								return m_andType;

							case GroupConstraint::Or:
								return m_orType;
						}

						break;
					}

					case Qt::TextAlignmentRole:
						return Qt::AlignCenter;
				}

			break;
		}

		default:
			break;
	}

	return QVariant();
}

IDM_PLUGIN_NS_END
