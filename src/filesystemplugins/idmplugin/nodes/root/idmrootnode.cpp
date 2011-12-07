#include "idmrootnode.h"
#include "items/idmrootnoderootitem.h"
#include "../query/idmnodequeryresults.h"
#include "../control/idmcopycontrol.h"
#include "../../gui/list/listentitydialog.h"
#include "../../gui/create/createentitydialog.h"
#include "../../gui/choose/choosefileentitydialog.h"
#include "../../gui/query/create/createquerydialog.h"
#include "../../../../application.h"
#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

template <typename T> inline T *value_cast(void *item, T *to);

template <> inline RootNodeRootItem *value_cast(void *item, RootNodeRootItem *to)
{
	Q_UNUSED(to);

	if (!static_cast<RootNodeRootItem::Base*>(item)->isList())
		if (static_cast<RootNodeItem*>(item)->isRoot())
			return static_cast<RootNodeRootItem*>(item);

	return 0;
}


IdmRootNode::IdmRootNode(const Info &storage, Node *parent) :
	TasksNode(m_itemsContainer, parent),
	m_info(storage.absolutePath()),
	m_container(storage),
	m_delegate(m_container),
	m_items(m_itemsContainer.m_container)
{
	m_items.push_back(new RootNodeRootItem());
}

int IdmRootNode::columnCount(const QModelIndex &parent) const
{
	return 1;
}

Qt::ItemFlags IdmRootNode::flags(const QModelIndex &index) const
{
	return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

QVariant IdmRootNode::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

bool IdmRootNode::isDir() const
{
	return true;
}

bool IdmRootNode::isFile() const
{
	return false;
}

bool IdmRootNode::exists() const
{
	return m_info.exists();
}

QString IdmRootNode::fileName() const
{
	return m_info.fileName();
}

QString IdmRootNode::absolutePath() const
{
	return m_info.absolutePath();
}

QString IdmRootNode::absoluteFilePath() const
{
	return m_info.absoluteFilePath();
}

QString IdmRootNode::absoluteFilePath(const QString &fileName) const
{
	return m_info.absoluteFilePath(fileName);
}

QDateTime IdmRootNode::lastModified() const
{
	return m_info.lastModified();
}

void IdmRootNode::refresh()
{

}

IFileInfo *IdmRootNode::info(const QModelIndex &idx) const
{
	return 0;
}

ICopyControl *IdmRootNode::createControl(INodeView *view) const
{
	if (IdmEntity *entity = ChooseFileEntityDialog::choose(m_container, &Application::instance()->mainWindow()))
		return new IdmCopyControl(m_container, entity, absoluteFilePath());
	else
		return 0;
}

void IdmRootNode::menuAction(QAction *action, INodeView *view)
{
	switch (static_cast<IdmContainer::MenuId>(action->data().toInt()))
	{
		case IdmContainer::Create:
		{
			CreateEntityDialog dialog(m_container, QString(), &Application::instance()->mainWindow());

			if (dialog.exec() == CreateEntityDialog::Accepted)
				if (m_container.transaction())
					if (IdmEntity *entity = m_container.createEntity(dialog.name(), dialog.type(), dialog.shortFormat()))
						if (entity->type() == Database::Composite)
						{
							bool ok = true;

							for (CreateEntityDialog::size_type i = 0, size = dialog.size(); i < size; ++i)
								if (!m_container.addProperty(entity, dialog.property(i), dialog.propertyName(i)))
								{
									ok = false;
									m_container.rollback();
									QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
									break;
								}

							if (ok && !m_container.commit())
							{
								m_container.rollback();
								QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
							}
						}
						else
						{
							if (!m_container.commit())
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
				else
					QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());

			break;
		}
		case IdmContainer::Find:
		{
			if (IdmEntity *entity = ChooseFileEntityDialog::choose(m_container, &Application::instance()->mainWindow()))
			{
				CreateQueryDialog dialog(m_container, entity, &Application::instance()->mainWindow());

				if (dialog.exec() == CreateQueryDialog::Accepted)
					switchTo(new IdmNodeQueryResults(m_container, dialog.query(), absoluteFilePath(), this), view);
			}

			break;
		}
		case IdmContainer::List:
		{
			if (m_container.transaction())
			{
				ListEntityDialog dialog(m_container, &Application::instance()->mainWindow());

				if (dialog.exec() == ListEntityDialog::Accepted)
				{
					if (!m_container.commit())
					{
						m_container.rollback();
						QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
					}
				}
				else
					m_container.rollback();
			}
			else
				QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());

			break;
		}
	}
}

void IdmRootNode::createFile(const QModelIndex &index, INodeView *view)
{

}

void IdmRootNode::createDirectory(const QModelIndex &index, INodeView *view)
{

}

void IdmRootNode::rename(const QModelIndex &index, INodeView *view)
{

}

void IdmRootNode::rename(const QModelIndexList &list, INodeView *view)
{

}

void IdmRootNode::remove(const QModelIndexList &list, INodeView *view)
{
//	if (m_container.transaction())
//		if (!processIndexList(list, IdmFunctors::callTo(this, &IdmRootNode::processRemoveItem)))
//			m_container.rollback();
//		else
//			if (!m_container.commit())
//			{
//				QMessageBox::critical(&Application::instance()->mainWindow(), tr("Error"), m_container.lastError());
//				m_container.rollback();
//			}
}

void IdmRootNode::cancel(const QModelIndexList &list, INodeView *view)
{

}

void IdmRootNode::calculateSize(const QModelIndexList &list, INodeView *view)
{

}

void IdmRootNode::pathToClipboard(const QModelIndexList &list, INodeView *view)
{

}

void IdmRootNode::copy(const INodeView *source, INodeView *destination)
{

}

void IdmRootNode::move(const INodeView *source, INodeView *destination)
{

}

QModelIndex IdmRootNode::rootIndex() const
{
	return createIndex(0, 0, m_items.at(0));
}

QAbstractItemModel *IdmRootNode::proxyModel() const
{
	return const_cast<IdmRootNode*>(this);
}

QAbstractItemDelegate *IdmRootNode::itemDelegate() const
{
	return const_cast<IdmRootNodeDelegate*>(&m_delegate);
}

const INodeView::MenuActionList &IdmRootNode::menuActions() const
{
	return m_container.menuActions();
}

Node *IdmRootNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	if (RootNodeRootItem *item = value_cast(idx.internalPointer(), item))
		return parentNode();
	else
		return 0;
}

Node *IdmRootNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
}

void IdmRootNode::updateProgressEvent(TaskNodeItem::Base *item, quint64 progress, quint64 timeElapsed)
{

}

void IdmRootNode::completedProgressEvent(TaskNodeItem::Base *item, quint64 timeElapsed)
{

}

IDM_PLUGIN_NS_END
