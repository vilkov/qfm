#include "idmrootnode.h"
#include "items/idmrootnoderootitem.h"
#include "items/idmrootnodefilesitem.h"
#include "items/idmrootnodeentityitem.h"
#include "items/idmrootnodepropertyitem.h"
#include "../query/idmnodequeryresults.h"
#include "../folder/idmfoldernode.h"
#include "../control/idmcopycontrol.h"
#include "../../gui/create/createentitydialog.h"
#include "../../gui/choose/chooseentitydialog.h"
#include "../../gui/query/create/createquerydialog.h"
#include "../../gui/value/list/editable/editablevaluelistdialog.h"
#include "../../../../application.h"

#include <QtGui/QMessageBox>


IDM_PLUGIN_NS_BEGIN

IdmRootNode::IdmRootNode(IFileContainer::Holder &container, Node *parent) :
	TasksNode(m_itemsContainer, parent),
	m_items(m_itemsContainer.m_container),
	m_container(container, false),
	m_delegate(m_container)
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

ICopyControl *IdmRootNode::createControl(INodeView *view) const
{
	if (IdmEntity *entity = ChooseEntityDialog::chooseFile(m_container, Application::mainWindow()))
	{
		ICopyControl::Holder dest(m_container.container()->createControl(view));

		if (dest)
			return new IdmCopyControl(dest, m_container, entity);
	}

	return NULL;
}

void IdmRootNode::contextMenu(const QModelIndexList &list, INodeView *view)
{

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

void IdmRootNode::removeToTrash(const QModelIndexList &list, INodeView *view)
{

}

::History::Entry *IdmRootNode::search(const QModelIndex &index, INodeView *view)
{
	return NULL;
}

void IdmRootNode::refresh()
{

}

QString IdmRootNode::title() const
{
	QString res = m_container.container()->location();
	return res.mid(res.lastIndexOf(QChar(L'/')));
}

QString IdmRootNode::location() const
{
	return m_container.container()->location();
}

IdmRootNode::Sorting IdmRootNode::sorting() const
{
	return Sorting(0, Qt::AscendingOrder);
}

IdmRootNode::Geometry IdmRootNode::geometry() const
{
	return Geometry() << 100;
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

QAbstractItemView::SelectionMode IdmRootNode::selectionMode() const
{
	return QAbstractItemView::SingleSelection;
}

::History::Entry *IdmRootNode::menuAction(QAction *action, INodeView *view)
{
	switch (static_cast<MenuId>(action->data().toInt()))
	{
		case Create:
		{
			createEntity();
			break;
		}

		case Remove:
		{
			removeEntity(view->currentIndex());
			break;
		}

		case AddProperty:
		{
			addProperty(view->currentIndex());
			break;
		}

		case RemoveProperty:
		{
			removeProperty(view->currentIndex());
			break;
		}

		case Find:
		{
			if (IdmEntity *entity = ChooseEntityDialog::chooseFile(m_container, Application::mainWindow()))
			{
				CreateQueryDialog dialog(m_container, entity, Application::mainWindow());

				if (dialog.exec() == CreateQueryDialog::Accepted)
					return switchTo(new IdmNodeQueryResults(m_container, dialog.query(), this), view);
			}

			break;
		}
	}

	return NULL;
}

QModelIndex IdmRootNode::rootIndex() const
{
	return createIndex(RootItemIndex, 0, m_items.at(RootItemIndex));
}

Node *IdmRootNode::viewChild(const QModelIndex &idx, QModelIndex &selected, bool newTab)
{
	RootNodeItem *item;

	if ((item = static_cast<RootNodeItem *>(idx.internalPointer()))->isEntity())
		if (m_container.transaction())
		{
			EditableValueListDialog dialog(
					m_container,
					Select(static_cast<RootNodeEntityItem *>(item)->entity()),
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

			m_container.updateListGeometry(static_cast<RootNodeEntityItem *>(item)->entity(), dialog.geometry());
		}
		else
			QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
	else
		if (item->isRoot())
			return parentNode();
		else
			if (item->isFiles())
			{
				if (static_cast<RootNodeFilesItem *>(item)->node() == NULL)
				{
					IFileContainer::Holder folder(m_container.container()->open());
					Node *node = new IdmFolderNode(folder, m_container, this);
					static_cast<RootNodeFilesItem *>(item)->setNode(node);
				}

				return static_cast<RootNodeFilesItem *>(item)->node();
			}

	return NULL;
}

Node *IdmRootNode::viewChild(const QString &fileName, QModelIndex &selected)
{
	if (Node *node = static_cast<RootNodeFilesItem *>(m_items.at(FilesItemIndex))->node())
		return static_cast<IdmFolderNode *>(node)->privateViewChild(fileName, selected);
	else
	{
		IFileContainer::Holder folder(m_container.container()->open());

		static_cast<RootNodeFilesItem *>(m_items.at(FilesItemIndex))->setNode(node = new IdmFolderNode(folder, m_container, this));
		return static_cast<IdmFolderNode *>(node)->privateViewChild(fileName, selected);
	}

	return NULL;
}

void IdmRootNode::nodeRemoved(Node *node)
{
	static_cast<RootNodeFilesItem *>(m_items.at(FilesItemIndex))->setNode(0);
}

void IdmRootNode::updateProgressEvent(const Item::Holder &item, quint64 progress, quint64 timeElapsed)
{

}

void IdmRootNode::completedProgressEvent(const Item::Holder &item, quint64 timeElapsed)
{

}

void IdmRootNode::performActionEvent(const AsyncFileAction::FilesList &files, const QString &error)
{

}

IdmRootNode::Container::Container()
{}

IdmRootNode::Container::~Container()
{
	qDeleteAll(m_container);
}

IdmRootNode::Container::size_type IdmRootNode::Container::size() const
{
	return m_container.size();
}

IdmRootNode::Container::Item *IdmRootNode::Container::at(size_type index) const
{
	return m_container.at(index);
}

IdmRootNode::Container::size_type IdmRootNode::Container::indexOf(Item *item) const
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
							doAdd(entity);
						else
						{
							QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
							m_container.rollback();
						}
				}
				else
					if (m_container.commit())
						doAdd(entity);
					else
					{
						QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
						m_container.rollback();
					}
			else
			{
				QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
				m_container.rollback();
			}
		else
			QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
}

void IdmRootNode::removeEntity(const QModelIndex &index)
{
	if (index.isValid() && static_cast<RootNodeItem *>(index.internalPointer())->isEntity())
	{
		RootNodeEntityItem *item = static_cast<RootNodeEntityItem *>(index.internalPointer());

		if (QMessageBox::question(
				Application::mainWindow(),
				tr("Remove entity"),
				tr("Would you like to remove entity \"%1\"?").
					arg(item->entity()->name()),
				QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			if (m_container.transaction())
				if (m_container.removeEntity(item->entity()))
				{
					doRemove(item->entity());

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
	}
}

void IdmRootNode::addProperty(const QModelIndex &index)
{
	if (index.isValid() && static_cast<RootNodeItem *>(index.internalPointer())->isEntity())
	{
		RootNodeEntityItem *item = static_cast<RootNodeEntityItem *>(index.internalPointer());

		if (item->entity()->type() == Database::Composite)
			if (m_container.transaction())
			{
				QString propertyName;

				if (IdmEntity *property = ChooseEntityDialog::chooseProperty(m_container, item->entity(), propertyName, Application::mainWindow()))
					if (m_container.addProperty(item->entity(), property, propertyName))
						if (m_container.commit())
							doAdd(index, item, property, propertyName);
						else
						{
							QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
							m_container.rollback();
						}
					else
					{
						QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
						m_container.rollback();
					}
				else
					m_container.rollback();
			}
			else
				QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
		else
			QMessageBox::warning(
					Application::mainWindow(),
					tr("Wrong entity"),
					tr("Entity \"%1\" is not composite.").
						arg(item->entity()->name()));
	}
}

void IdmRootNode::removeProperty(const QModelIndex &index)
{
	if (index.isValid() && static_cast<RootNodeItem *>(index.internalPointer())->isProperty())
	{
		RootNodePropertyItem *property = static_cast<RootNodePropertyItem *>(index.internalPointer());
		RootNodeEntityItem *item = static_cast<RootNodeEntityItem *>(property->parent());

		if (QMessageBox::question(
				Application::mainWindow(),
				tr("Remove property"),
				tr("Would you like to remove property \"%1 (%2)\" from \"%3\" entity?").
					arg(property->name()).
					arg(property->entity()->name()).
					arg(item->entity()->name()),
				QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
		{
			if (m_container.transaction())
				if (m_container.removeProperty(item->entity(), property->entity()))
					if (m_container.commit())
						doRemove(index, item, property);
					else
					{
						QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
						m_container.rollback();
					}
				else
				{
					QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
					m_container.rollback();
				}
			else
				QMessageBox::critical(Application::mainWindow(), tr("Error"), m_container.lastError());
		}
	}
}

void IdmRootNode::doAdd(IdmEntity *entity)
{
	RootNodeEntityItem *item;

	beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
	m_items.push_back(item = new RootNodeEntityItem(entity));
	m_entities[entity].push_back(item);
	expand(item);
	endInsertRows();
}

void IdmRootNode::doRemove(IdmEntity *entity)
{
	Container::size_type row;
	Container::Item *parent;
	Container::List list = m_entities.take(entity);
	QModelIndex index;

	for (Container::List::size_type i = 0, size = list.size(); i < size; ++i)
	{
		index = IdmRootNode::parent(list.at(i), row);
		beginRemoveRows(index, row, row);

		if (parent = list.at(i)->parent())
			static_cast<RootNodeEntityItem *>(parent)->remove(row);
		else
			delete m_items.takeAt(row);

		endRemoveRows();
	}
}

void IdmRootNode::doAdd(const QModelIndex &index, Container::Item *item, IdmEntity *property, const QString &propertyName)
{
	RootNodeEntityItem *child;

	beginInsertRows(index, static_cast<RootNodeEntityItem *>(item)->size(), static_cast<RootNodeEntityItem *>(item)->size());
	static_cast<RootNodeEntityItem *>(item)->add(child = new RootNodePropertyItem(property, propertyName, item));
	m_entities[property].push_back(child);
	expand(child);
	endInsertRows();
}

void IdmRootNode::doRemove(const QModelIndex &index, Container::Item *item, Container::Item *property)
{
	Container::Item::size_type idx = item->indexOf(property);

	beginRemoveRows(parent(index), idx, idx);
	Container::List &items = m_entities[static_cast<RootNodeEntityItem *>(item)->entity()];
	items.removeAt(items.indexOf(item));
	static_cast<RootNodeEntityItem *>(item)->remove(idx);
	endRemoveRows();
}

void IdmRootNode::expand(Container::Item *p)
{
	RootNodeEntityItem *item;
	RootNodeEntityItem *parent = static_cast<RootNodeEntityItem *>(p);

	for (IdmEntity::size_type i = 0, size = parent->entity()->size(); i < size; ++i)
	{
		parent->add(item = new RootNodePropertyItem(parent->entity()->at(i), parent));
		m_entities[item->entity()].push_back(item);
		expand(item);
	}
}

IDM_PLUGIN_NS_END
