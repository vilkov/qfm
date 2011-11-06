#include "idmnodequeryresults.h"
#include "items/idmqueryresultrootitem.h"
#include "items/idmqueryresultpathvalueitem.h"
#include "functors/idmqueryresultsfunctor.h"
#include "../../../../tools/widgets/stringdialog/stringdialog.h"
#include "../../../../application.h"
#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmNodeQueryResults::IdmNodeQueryResults(const IdmContainer &container, const Select &query, const Info &info, Node *parent) :
	TasksNode(m_itemsContainer, parent),
	m_items(m_itemsContainer.m_container),
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
	if (static_cast<IdmItem*>(idx.internalPointer())->isList())
		return 0;
	else
		if (static_cast<QueryResultValueItem*>(idx.internalPointer())->isPath())
			return &static_cast<QueryResultPathValueItem*>(idx.internalPointer())->info();
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

void IdmNodeQueryResults::createFile(const QModelIndex &index)
{

}

void IdmNodeQueryResults::createDirectory(const QModelIndex &index)
{

}

void IdmNodeQueryResults::rename(const QModelIndexList &list)
{
	process(list, callTo(this, &IdmNodeQueryResults::doRename));
}

void IdmNodeQueryResults::remove(const QModelIndexList &list)
{

}

void IdmNodeQueryResults::cancel(const QModelIndexList &list)
{

}

void IdmNodeQueryResults::calculateSize(const QModelIndexList &list)
{

}

void IdmNodeQueryResults::pathToClipboard(const QModelIndexList &list)
{

}

void IdmNodeQueryResults::copy(const QModelIndexList &list, INode *destination)
{

}

void IdmNodeQueryResults::move(const QModelIndexList &list, INode *destination)
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
	return const_cast<IdmDelegate*>(&m_delegate);
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
		if (!static_cast<IdmItem*>(list.at(i).internalPointer())->isList())
			functor(list.at(i), static_cast<QueryResultValueItem*>(list.at(i).internalPointer()));
}

void IdmNodeQueryResults::doRename(const QModelIndex &index, QueryResultValueItem *value)
{
	if (value->isPath())
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
}

IDM_PLUGIN_NS_END
