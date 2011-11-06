#include "idmnodequeryresults.h"
#include "items/idmqueryresultrootitem.h"
#include "items/idmqueryresultpathvalueitem.h"
#include "functors/idmqueryresultsfunctor.h"
#include "../../../../tools/widgets/stringdialog/stringdialog.h"
#include "../../../../application.h"
#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

template <typename T> inline T *value_cast(void *item, T *to)
{
	return 0;
}

template <> inline QueryResultValueItem *value_cast(void *item, QueryResultValueItem *to)
{
	Q_UNUSED(to);

	if (static_cast<IdmItem*>(item)->isList())
		return 0;
	else
		return static_cast<QueryResultValueItem*>(item);
}

template <> inline QueryResultPathValueItem *value_cast(void *item, QueryResultPathValueItem *to)
{
	Q_UNUSED(to);

	if (static_cast<IdmItem*>(item)->isList())
		return 0;
	else
		if (static_cast<QueryResultValueItem*>(item)->value()->entity()->type() == Database::Path)
			return static_cast<QueryResultPathValueItem*>(item);
		else
			return 0;
}


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

ICopyControl *IdmNodeQueryResults::createControl() const
{
	return 0;
}

void IdmNodeQueryResults::menuAction(QAction *action, INodeView *view)
{

}

void IdmNodeQueryResults::createFile(const QModelIndex &index, INodeView *view)
{

}

void IdmNodeQueryResults::createDirectory(const QModelIndex &index, INodeView *view)
{

}

void IdmNodeQueryResults::rename(const QModelIndexList &list, INodeView *view)
{
	process(list, callTo(this, view, &IdmNodeQueryResults::doRename));
}

void IdmNodeQueryResults::remove(const QModelIndexList &list, INodeView *view)
{

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

void IdmNodeQueryResults::copy(const QModelIndexList &list, INode *destination, INodeView *view)
{

}

void IdmNodeQueryResults::move(const QModelIndexList &list, INode *destination, INodeView *view)
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

void IdmNodeQueryResults::process(const QModelIndexList &list, const Functor &functor)
{
	for (QModelIndexList::size_type i = 0, size = list.size(); i < size; ++i)
		if (QueryResultValueItem *item = value_cast(list.at(i).internalPointer(), item))
			functor(list.at(i), item);
}

void IdmNodeQueryResults::doRename(INodeView *view, const QModelIndex &index, QueryResultValueItem *value)
{
	if (value->value()->entity()->type() == Database::Path)
	{
		if (m_container.transaction())
		{
			QueryResultPathValueItem *file = static_cast<QueryResultPathValueItem*>(value);
			StringDialog dialog(
					tr("Enter new name for file \"%1\"").arg(file->info().fileName()),
					tr("Name"),
					file->info().fileName(),
					&Application::instance()->mainWindow());

			if (dialog.exec() == QDialog::Accepted)
			{
				QString error;
				QString fileName = file->info().fileName();

				if (file->info().rename(dialog.value(), error))
					if (m_container.updateValue(file->value(), file->info().absoluteFilePath(dialog.value())))
						if (m_container.commit())
						{
							file->update();
							emit dataChanged(index, index);
						}
						else
						{
							m_container.rollback();
							file->info().rename(fileName, error);
							QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
						}
					else
					{
						m_container.rollback();
						file->info().rename(fileName, error);
						QMessageBox::critical(
									&Application::instance()->mainWindow(),
									tr("Failed to rename file \"%1\"").arg(file->info().fileName()),
									m_container.lastError());
					}
				else
				{
					m_container.rollback();
					QMessageBox::critical(
								&Application::instance()->mainWindow(),
								tr("Failed to rename file \"%1\"").arg(file->info().fileName()),
								error);
				}
			}
			else
				m_container.rollback();
		}
		else
			QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
	}
	else
		if (value->value()->entity()->type() == Database::Composite)
		{

		}
		else
			view->edit(index);
}

IDM_PLUGIN_NS_END
