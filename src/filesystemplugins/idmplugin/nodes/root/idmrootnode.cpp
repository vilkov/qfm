#include "idmrootnode.h"
#include "items/idmrootnoderootitem.h"
#include "items/idmrootnodefilesitem.h"
#include "items/idmrootnodeentityitem.h"
#include "items/idmrootnodepropertyitem.h"
#include "control/idmcopycontrol.h"
#include "../query/idmnodequeryresults.h"
#include "../folder/idmfoldernode.h"
#include "../../gui/list/listentitydialog.h"
#include "../../gui/create/createentitydialog.h"
#include "../../gui/choose/choosefileentitydialog.h"
#include "../../gui/query/create/createquerydialog.h"
#include "../../../../application.h"
#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

template <typename T> inline T *value_cast(void *item, T *to = 0);

template <> inline RootNodeRootItem *value_cast(void *item, RootNodeRootItem *to)
{
	Q_UNUSED(to);

	if (!static_cast<RootNodeRootItem::Base*>(item)->isList())
		if (static_cast<RootNodeItem*>(item)->isRoot())
			return static_cast<RootNodeRootItem*>(item);

	return 0;
}

template <> inline RootNodeFilesItem *value_cast(void *item, RootNodeFilesItem *to)
{
	Q_UNUSED(to);

	if (!static_cast<RootNodeFilesItem::Base*>(item)->isList())
		if (static_cast<RootNodeItem*>(item)->isFiles())
			return static_cast<RootNodeFilesItem*>(item);

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
	m_items.push_back(new RootNodeFilesItem());

    for (IdmContainer::size_type i = 0, size = m_container.size(); i < size; ++i)
    	doAdd(m_container.at(i));
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
	return createIndex(RootItem, 0, m_items.at(RootItem));
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
	if (RootNodeItem *item = value_cast<RootNodeRootItem>(idx.internalPointer()))
		return parentNode();
	else
		if (item = value_cast<RootNodeFilesItem>(idx.internalPointer()))
		{
			m_info.refresh();

			if (m_info.exists())
			{
				if (static_cast<RootNodeFilesItem*>(item)->node())
					static_cast<RootNodeFilesItem*>(item)->node()->setParentEntryIndex(idx);
				else
				{
					Node *node = new IdmFolderNode(m_info, this);

					node->setParentEntryIndex(idx);
					static_cast<RootNodeFilesItem*>(item)->setNode(node);
				}

				return static_cast<RootNodeFilesItem*>(item)->node();
			}
		}

	return 0;
}

Node *IdmRootNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	m_info.refresh();

	if (m_info.exists())
		if (Node *node = static_cast<RootNodeFilesItem*>(m_items.at(FilesItem))->node())
			return static_cast<IdmFolderNode*>(node)->privateViewChild(fileName, plugins, selected);
		else
		{
			static_cast<RootNodeFilesItem*>(m_items.at(FilesItem))->setNode(node = new IdmFolderNode(m_info, this));
			return static_cast<IdmFolderNode*>(node)->privateViewChild(fileName, plugins, selected);
		}

	return 0;
}

void IdmRootNode::nodeRemoved(Node *node)
{
	static_cast<RootNodeFilesItem*>(m_items.at(FilesItem))->setNode(0);
}

void IdmRootNode::updateProgressEvent(TaskNodeItem::Base *item, quint64 progress, quint64 timeElapsed)
{

}

void IdmRootNode::completedProgressEvent(TaskNodeItem::Base *item, quint64 timeElapsed)
{

}

IdmRootNode::ItemsContainer::ItemsContainer() :
	ModelContainer()
{}

IdmRootNode::ItemsContainer::~ItemsContainer()
{
	qDeleteAll(m_container);
}

IdmRootNode::ItemsContainer::size_type IdmRootNode::ItemsContainer::size() const
{
	return m_container.size();
}

IdmRootNode::ItemsContainer::Item *IdmRootNode::ItemsContainer::at(size_type index) const
{
	return m_container.at(index);
}

IdmRootNode::ItemsContainer::size_type IdmRootNode::ItemsContainer::indexOf(Item *item) const
{
	return m_container.indexOf(item);
}

void IdmRootNode::add(IdmEntity *entity)
{
	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	doAdd(entity);
	endInsertRows();
}

void IdmRootNode::remove(const QModelIndex &index)
{
//	beginRemoveRows(QModelIndex(), index.row(), index.row());
//	delete m_items.at(index.row());
//	m_items.remove(index.row());
//	endRemoveRows();
}

void IdmRootNode::doAdd(IdmEntity *entity)
{
	RootNodeEntityItem *item;

	m_items.push_back(item = new RootNodeEntityItem(entity));
	m_entities[entity].push_back(item);
	expand(item);
}

void IdmRootNode::doAdd(ItemsContainer::Item *item, IdmEntity *property)
{
	RootNodeEntityItem *child;

	static_cast<RootNodeEntityItem*>(item)->add(child = new RootNodeEntityItem(property, item));
	m_entities[property].push_back(child);
	expand(child);
}

void IdmRootNode::doRemove(ItemsContainer::Item *item, ItemsContainer::size_type index)
{
	ItemsContainer::List &items = m_entities[static_cast<RootNodeEntityItem*>(item)->entity()];
	items.removeAt(items.indexOf(item));
	static_cast<RootNodeEntityItem*>(item->parent())->remove(index);
}

void IdmRootNode::expand(ItemsContainer::Item *p)
{
	IdmEntity *entity;
	RootNodeEntityItem *item;
	RootNodeEntityItem *parent = static_cast<RootNodeEntityItem*>(p);

	for (IdmEntity::size_type i = 0, size = parent->entity()->size(); i < size; ++i)
	{
		parent->add(item = new RootNodeEntityItem(entity = parent->entity()->at(i).entity, parent));
		m_entities[entity].push_back(item);
		expand(item);
	}
}

IDM_PLUGIN_NS_END
