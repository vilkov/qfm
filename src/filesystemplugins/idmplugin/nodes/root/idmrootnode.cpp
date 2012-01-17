#include "idmrootnode.h"
#include "items/idmrootnoderootitem.h"
#include "items/idmrootnodefilesitem.h"
#include "items/idmrootnodeentityitem.h"
#include "items/idmrootnodepropertyitem.h"
#include "control/idmcopycontrol.h"
#include "../query/idmnodequeryresults.h"
#include "../folder/idmfoldernode.h"
#include "../../gui/create/createentitydialog.h"
#include "../../gui/choose/choosefileentitydialog.h"
#include "../../gui/query/create/createquerydialog.h"
#include "../../gui/value/list/editable/editablevaluelistdialog.h"
#include "../../../../application.h"
#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

template <typename T> inline T *value_cast(void *item, T *to = 0);

template <> inline RootNodeRootItem *value_cast(void *item, RootNodeRootItem *to)
{
	Q_UNUSED(to);

	if (static_cast<RootNodeItem*>(item)->isRoot())
		return static_cast<RootNodeRootItem*>(item);

	return 0;
}

template <> inline RootNodeFilesItem *value_cast(void *item, RootNodeFilesItem *to)
{
	Q_UNUSED(to);

	if (static_cast<RootNodeItem*>(item)->isFiles())
		return static_cast<RootNodeFilesItem*>(item);

	return 0;
}


IdmRootNode::IdmRootNode(const Info &storage, Node *parent) :
	TasksNode(m_itemsContainer, parent),
	m_items(m_itemsContainer.m_container),
	m_container(storage),
	m_delegate(m_container),
	m_info(storage.absolutePath())
{
	m_actions.push_back(new QAction(tr("Create"), 0));
	m_actions.last()->setData(Create);

	m_actions.push_back(new QAction(tr("Remove"), 0));
	m_actions.last()->setData(Remove);

	m_actions.push_back(new QAction(tr("AddProperty"), 0));
	m_actions.last()->setData(AddProperty);

	m_actions.push_back(new QAction(tr("RemoveProperty"), 0));
	m_actions.last()->setData(RemoveProperty);

	m_actions.push_back(new QAction(tr("Find"), 0));
	m_actions.last()->setData(Find);


	m_items.push_back(new RootNodeRootItem());
	m_items.push_back(new RootNodeFilesItem());

    for (IdmContainer::size_type i = 0, size = m_container.size(); i < size; ++i)
    	doAdd(m_container.at(i));
}

IdmRootNode::~IdmRootNode()
{
	qDeleteAll(m_actions);
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

FileTypeId IdmRootNode::id() const
{
	return m_info.id();
}

QIcon IdmRootNode::icon() const
{
	return m_info.icon();
}

QString IdmRootNode::name() const
{
	return m_info.name();
}

QString IdmRootNode::description() const
{
	return m_info.description();
}

bool IdmRootNode::isDir() const
{
	return true;
}

bool IdmRootNode::isFile() const
{
	return false;
}

bool IdmRootNode::isLink() const
{
	return false;
}

bool IdmRootNode::exists() const
{
	return m_info.exists();
}

qint64 IdmRootNode::fileSize() const
{
	return m_info.fileSize();
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

int IdmRootNode::permissions() const
{
	return m_info.permissions();
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
	if (IdmEntity *entity = ChooseFileEntityDialog::choose(m_container, Application::mainWindow()))
		return new IdmCopyControl(m_container, entity, absoluteFilePath());
	else
		return 0;
}

void IdmRootNode::menuAction(QAction *action, INodeView *view)
{
	switch (static_cast<MenuId>(action->data().toInt()))
	{
		case Create:
		{
			createEntity();
			break;
		}

		case Find:
		{
			if (IdmEntity *entity = ChooseFileEntityDialog::choose(m_container, Application::mainWindow()))
			{
				CreateQueryDialog dialog(m_container, entity, Application::mainWindow());

				if (dialog.exec() == CreateQueryDialog::Accepted)
					switchTo(new IdmNodeQueryResults(m_container, dialog.query(), absoluteFilePath(), this), view);
			}

			break;
		}
	}
}

void IdmRootNode::createFile(const QModelIndex &index, INodeView *view)
{
//	RootNodeItem::Base *item;
//
//	if ((item = static_cast<RootNodeItem::Base*>(index.internalPointer()))->isList())
//	{
//
//	}
//	else
//		createEntity();
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
//				QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
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

QAbstractItemModel *IdmRootNode::model() const
{
	return const_cast<IdmRootNode*>(this);
}

QAbstractItemDelegate *IdmRootNode::delegate() const
{
	return const_cast<IdmRootNodeDelegate*>(&m_delegate);
}

const INodeView::MenuActionList &IdmRootNode::actions() const
{
	return m_actions;
}

QModelIndex IdmRootNode::rootIndex() const
{
	return createIndex(RootItemIndex, 0, m_items.at(RootItemIndex));
}

Node *IdmRootNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	RootNodeItem *item;

	if ((item = static_cast<RootNodeItem*>(idx.internalPointer()))->isEntity())
		if (m_container.transaction())
		{
			EditableValueListDialog dialog(
					m_container,
					Select(static_cast<RootNodeEntityItem*>(item)->entity()),
					Application::mainWindow());

			if (dialog.exec() == EditableValueListDialog::Accepted)
			{
				if (!m_container.commit())
				{
					QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
					m_container.rollback();
				}
			}
			else
				m_container.rollback();
		}
		else
			QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
	else
		if (item->isRoot())
			return parentNode();
		else
			if (item->isFiles())
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
		if (Node *node = static_cast<RootNodeFilesItem*>(m_items.at(FilesItemIndex))->node())
			return static_cast<IdmFolderNode*>(node)->privateViewChild(fileName, plugins, selected);
		else
		{
			static_cast<RootNodeFilesItem*>(m_items.at(FilesItemIndex))->setNode(node = new IdmFolderNode(m_info, this));
			return static_cast<IdmFolderNode*>(node)->privateViewChild(fileName, plugins, selected);
		}

	return 0;
}

void IdmRootNode::nodeRemoved(Node *node)
{
	static_cast<RootNodeFilesItem*>(m_items.at(FilesItemIndex))->setNode(0);
}

void IdmRootNode::updateProgressEvent(const TaskNodeItem *item, quint64 progress, quint64 timeElapsed)
{

}

void IdmRootNode::completedProgressEvent(const TaskNodeItem *item, quint64 timeElapsed)
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

void IdmRootNode::createEntity()
{
	CreateEntityDialog dialog(m_container, QString(), Application::mainWindow());

	if (dialog.exec() == CreateEntityDialog::Accepted)
		if (m_container.transaction())
			if (IdmEntity *entity = m_container.createEntity(dialog.name(), dialog.type(), dialog.shortFormat()))
				if (entity->type() == Database::Composite)
				{
					bool ok = true;

					for (CreateEntityDialog::size_type i = 0, size = dialog.size(); i < size; ++i)
						if (!m_container.addProperty(entity, dialog.property(i), dialog.propertyName(i)))
						{
							QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
							m_container.rollback();
							ok = false;
							break;
						}

					if (ok)
						if (m_container.commit())
							add(entity);
						else
						{
							QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
							m_container.rollback();
						}
				}
				else
				{
					if (!m_container.commit())
					{
						QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
						m_container.rollback();
					}
				}
			else
			{
				QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
				m_container.rollback();
			}
		else
			QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
}

void IdmRootNode::addProperty(ItemsContainer::Item *item)
{

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
	RootNodeEntityItem *item;
	RootNodeEntityItem *parent = static_cast<RootNodeEntityItem*>(p);

	for (IdmEntity::size_type i = 0, size = parent->entity()->size(); i < size; ++i)
	{
		parent->add(item = new RootNodePropertyItem(parent->entity()->at(i), parent));
		m_entities[item->entity()].push_back(item);
		expand(item);
	}
}

IDM_PLUGIN_NS_END
