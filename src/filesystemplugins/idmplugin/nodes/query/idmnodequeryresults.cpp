#include "idmnodequeryresults.h"
#include "items/idmqueryresultvaluecast.h"
#include "functors/idmqueryresultsfunctor.h"
#include "control/idmqueryresultscopycontrol.h"
#include "../../gui/tools/idmentityvaluecreationtools.h"
#include "../../../../tools/rangeintersection.h"
#include "../../../../tools/widgets/stringdialog/stringdialog.h"
#include "../../../../filesystem/tools/filesystemcommontools.h"
#include "../../../../application.h"
#include <QtGui/QMessageBox>
#include <QtCore/QDebug>


IDM_PLUGIN_NS_BEGIN


class SelectValuesFunctor : public Functor
{
public:
	SelectValuesFunctor()
	{

	}
};


IdmNodeQueryResults::IdmNodeQueryResults(const IdmContainer &container, const Select &query, const Info &info, Node *parent) :
	TasksNode(m_itemsContainer, parent),
	m_items(m_itemsContainer.m_container),
	m_delegate(container),
	m_container(container),
	m_reader(m_container, query),
	m_info(info),
	m_label(tr("Found \"%1\" entities...").arg(m_reader.entity()->name()))
{}

int IdmNodeQueryResults::columnCount(const QModelIndex &parent) const
{
	return 1;
}

void IdmNodeQueryResults::fetchMore(const QModelIndex &parent)
{
	IdmEntityValue *item;
	IdmModelContainer::Container list;

	list.reserve(PrefetchLimit);

	for (qint32 actualLimit = 0; actualLimit < PrefetchLimit; ++actualLimit)
		if (item = m_reader.next())
			list.push_back(new QueryResultRootItem(item));
		else
			break;

	if (!list.isEmpty())
	{
		beginInsertRows(parent, m_items.size(), m_items.size() + list.size() - 1);
		m_items.append(list);
		endInsertRows();
	}
}

bool IdmNodeQueryResults::canFetchMore(const QModelIndex &parent) const
{
    return !parent.isValid() && !m_reader.eof();
}

Qt::ItemFlags IdmNodeQueryResults::flags(const QModelIndex &index) const
{
//	if (QueryResultValueItem *item = value_cast(index.internalPointer(), item))
//		if (item->value()->entity()->type() != Database::Composite &&
//			item->value()->entity()->type() != Database::Path)
			return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;

//	return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

QVariant IdmNodeQueryResults::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (section == 0 && orientation == Qt::Horizontal)
		switch (role)
		{
			case Qt::DisplayRole:
				return m_label;

			case Qt::TextAlignmentRole:
				return Qt::AlignCenter;

			default:
				break;
		}

	return QVariant();
}

bool IdmNodeQueryResults::isDir() const
{
	return false;
}

bool IdmNodeQueryResults::isFile() const
{
	return false;
}

bool IdmNodeQueryResults::exists() const
{
	return true;
}

QString IdmNodeQueryResults::fileName() const
{
	return m_info.fileName();
}

QString IdmNodeQueryResults::absolutePath() const
{
	return m_info.absolutePath();
}

QString IdmNodeQueryResults::absoluteFilePath() const
{
	return m_info.absoluteFilePath();
}

QString IdmNodeQueryResults::absoluteFilePath(const QString &fileName) const
{
	return m_info.absoluteFilePath(fileName);
}

QDateTime IdmNodeQueryResults::lastModified() const
{
	return m_info.lastModified();
}

void IdmNodeQueryResults::refresh()
{

}

IFileInfo *IdmNodeQueryResults::info(const QModelIndex &idx) const
{
	if (QueryResultPathValueItem *item = value_cast(idx.internalPointer(), item))
		return &item->info();
	else
		return 0;
}

ICopyControl *IdmNodeQueryResults::createControl(INodeView *view) const
{
	QModelIndex index = view->currentIndex();

	if (QueryResultPropertyItem *item = value_cast(index.internalPointer(), item))
		if (item->property().entity->type() == Database::Path)
			if (item->size() == 0)
				return new IdmQueryResultsCopyControl(
						m_container,
						item->rootValue(),
						item->property(),
						const_cast<IdmNodeQueryResults*>(this),
						index,
						m_info);
			else
			{
				QString destination;
				Tools::DestinationFromPathList tree;

				for (QueryResultPropertyItem::size_type i = 0, size = item->size(); i < size; ++i)
					tree.add(static_cast<QueryResultValueItem*>(item->at(i))->value()->value().toString());

				if (!(destination = tree.choose(&Application::instance()->mainWindow())).isEmpty())
					return new IdmQueryResultsCopyControl(
							m_container,
							item->rootValue(),
							item->property(),
							const_cast<IdmNodeQueryResults*>(this),
							index,
							destination);
			}

	return 0;
}

void IdmNodeQueryResults::menuAction(QAction *action, INodeView *view)
{

}

void IdmNodeQueryResults::createFile(const QModelIndex &index, INodeView *view)
{
	if (QueryResultPropertyItem *item = value_cast(index.internalPointer(), item))
		if (m_container.transaction())
		{
			bool declined = false;
			PScopedPointer<IdmEntityValue> value(
					CreationTools::chooseOrCreateValue(
					&Application::instance()->mainWindow(),
					m_container,
					item->property().entity,
					declined));

			if (value)
				if (m_container.addValue(item->rootValue(), value.data()))
					if (m_container.commit())
					{
						beginInsertRows(index, item->size(), item->size());
						item->add(value.take());
						endInsertRows();
					}
					else
					{
						m_container.rollback();
						QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
					}
				else
				{
					m_container.rollback();
					QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
				}
			else
			{
				m_container.rollback();

				if (!declined)
					QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
			}
		}
		else
			QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
}

void IdmNodeQueryResults::createDirectory(const QModelIndex &index, INodeView *view)
{

}

void IdmNodeQueryResults::rename(const QModelIndex &index, INodeView *view)
{
	view->edit(index);
}

void IdmNodeQueryResults::rename(const QModelIndexList &list, INodeView *view)
{

}

void IdmNodeQueryResults::remove(const QModelIndexList &list, INodeView *view)
{
	typedef QMap<QModelIndex, RangeIntersection> Map;

	if (m_container.transaction())
	{
		Map processed;
		QueryResultValueItem *item;

//		QueryResultPropertyItem *property = static_cast<QueryResultPropertyItem*>(value->parent());

		for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
			if (item = value_cast(list.at(i).internalPointer(), item))
			{

			}

//		if (m_container.removeValue(property->rootValue(), value->value()))
//		{
//			beginRemoveRows(FileSystemModel::parent(index), index.row(), index.row());
//
//			if (m_container.commit())
//				property->remove(index.row());
//			else
//			{
//				m_container.rollback();
//				QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
//			}
//
//			endRemoveRows();
//		}
//		else
//		{
//			m_container.rollback();
//			QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
//		}
	}
	else
		QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());



	process(list, callTo(this, view, &IdmNodeQueryResults::doRemove));
}

void IdmNodeQueryResults::cancel(const QModelIndexList &list, INodeView *view)
{

}

void IdmNodeQueryResults::calculateSize(const QModelIndexList &list, INodeView *view)
{

}

void IdmNodeQueryResults::pathToClipboard(const QModelIndexList &list, INodeView *view)
{

}

void IdmNodeQueryResults::copy(const INodeView *source, INodeView *destination)
{

}

void IdmNodeQueryResults::move(const INodeView *source, INodeView *destination)
{

}

QModelIndex IdmNodeQueryResults::rootIndex() const
{
	return QModelIndex();
}

QAbstractItemModel *IdmNodeQueryResults::proxyModel() const
{
	return const_cast<IdmNodeQueryResults*>(this);
}

QAbstractItemDelegate *IdmNodeQueryResults::itemDelegate() const
{
	return const_cast<IdmQueryResultsDelegate*>(&m_delegate);
}

const INodeView::MenuActionList &IdmNodeQueryResults::menuActions() const
{
	return m_actions;
}

Node *IdmNodeQueryResults::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
}

Node *IdmNodeQueryResults::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
}

void IdmNodeQueryResults::add(const QModelIndex &index, const IdmCompositeEntityValue::List &values)
{
	if (QueryResultPropertyItem *item = value_cast(index.internalPointer(), item))
	{
		beginInsertRows(index, item->size(), item->size() + values.size() - 1);
		item->add(values);
		endInsertRows();
	}
}

void IdmNodeQueryResults::remove(const QModelIndex &index, const IdmCompositeEntityValue::List &values)
{

}

void IdmNodeQueryResults::process(const QModelIndexList &list, const Functor &functor)
{
	QueryResultValueItem *item;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (item = value_cast(list.at(i).internalPointer(), item))
			functor(list.at(i), item);
}

void IdmNodeQueryResults::processRemove(const QModelIndexList &list, const Functor &functor)
{
	QueryResultValueItem *item;

	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (item = value_cast(list.at(i).internalPointer(), item))
			functor(list.at(i), item);
}

void IdmNodeQueryResults::doRemove(INodeView *view, const QModelIndex &index, QueryResultValueItem *value)
{
	if (m_container.transaction())
	{
		QueryResultPropertyItem *property = static_cast<QueryResultPropertyItem*>(value->parent());

		if (m_container.removeValue(property->rootValue(), value->value()))
		{
			beginRemoveRows(FileSystemModel::parent(index), index.row(), index.row());

			if (m_container.commit())
				property->remove(index.row());
			else
			{
				m_container.rollback();
				QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
			}

			endRemoveRows();
		}
		else
		{
			m_container.rollback();
			QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
		}
	}
	else
	{
		m_container.rollback();
		QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
	}
}

IDM_PLUGIN_NS_END
