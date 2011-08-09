#include "idmrootnode.h"
#include "items/idmmenu.h"
#include "items/idmroot.h"
#include "items/idmmessage.h"
#include "items/idmseparator.h"


FILE_SYSTEM_NS_BEGIN

struct MainMenuItems
{
	enum
	{
		Create,
		Remove
	};
};


IdmRootNode::IdmRootNode(const QFileInfo &storage, Node *parent) :
	Node(parent),
	m_info(storage.absolutePath()),
	m_storage(storage),
	m_updating(false)
{
	if (m_storage.isValid())
	{
		IdmMenu *submenu;
		QScopedPointer<IdmMenu> menu(new IdmMenu(tr("Menu"), tr("Main menu")));

		menu->add(MainMenuItems::Create, tr("Create"), tr("Create an entity"));
		menu->add(MainMenuItems::Remove, tr("Remove"), tr("Remove an entity"));
		submenu = menu->add(tr("List"), tr("List of the all entities"));
		submenu->add(0, tr("Some items"), tr("123"));

		m_items.push_back(menu.take());
		m_items.push_back(new IdmRoot(storage.absolutePath()));
	}
	else
	{
		m_items.push_back(new IdmSeparator());
		m_items.push_back(new IdmMessage(m_storage.lastError()));
	}

	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);
}

int IdmRootNode::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		if (static_cast<IdmItem*>(parent.internalPointer())->isList())
			return static_cast<IdmItemsList*>(parent.internalPointer())->size();
		else
			return 0;
	else
    	return m_items.size();
}

int IdmRootNode::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant IdmRootNode::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<IdmItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_items.at(index.row())->data(index.column(), role);
}

Qt::ItemFlags IdmRootNode::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant IdmRootNode::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex IdmRootNode::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		if (parent.isValid() && static_cast<IdmItem*>(parent.internalPointer())->isList())
			return createIndex(row, column, static_cast<IdmItemsList*>(parent.internalPointer())->at(row));
		else
			return createIndex(row, column, m_items.at(row));
    else
        return QModelIndex();
}

QModelIndex IdmRootNode::parent(const QModelIndex &child) const
{
    if (child.isValid())
		if (IdmItem *parent = static_cast<IdmItem*>(child.internalPointer())->parent())
			if (parent->parent())
				return createIndex(static_cast<IdmItemsList*>(parent->parent())->indexOf(parent), 0, parent);
			else
				return createIndex(m_items.indexOf(parent), 0, parent);

    return QModelIndex();
}

INode *IdmRootNode::root() const
{
	if (INode *res = static_cast<Node*>(Node::parent()))
		return res->root();
	else
		return (INode*)this;
}

int IdmRootNode::columnCount() const
{
	return 2;
}

IFileInfo *IdmRootNode::info(const QModelIndex &idx) const
{
	return 0;
}

IFileControl *IdmRootNode::createControl() const
{
	return 0;
}

IFileControl *IdmRootNode::createControl(const QModelIndex &idx, PluginsManager *plugins)
{
	return 0;
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
	m_info.refresh();
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
	return QString();
}

QDateTime IdmRootNode::lastModified() const
{
	return m_info.lastModified();
}

void IdmRootNode::refresh()
{

}

void IdmRootNode::remove(const QModelIndexList &list)
{

}

void IdmRootNode::cancel(const QModelIndexList &list)
{

}

void IdmRootNode::calculateSize(const QModelIndexList &list)
{

}

void IdmRootNode::pathToClipboard(const QModelIndexList &list)
{

}

void IdmRootNode::copy(const QModelIndexList &list, INode *destination)
{

}

void IdmRootNode::move(const QModelIndexList &list, INode *destination)
{

}

void IdmRootNode::viewClosed(INodeView *nodeView)
{
	removeView(nodeView);
}

void IdmRootNode::viewParent(INodeView *nodeView)
{
//	if (!m_info.isRoot())
//		if (exists())
//			switchTo(static_cast<Node*>(Node::parent()), nodeView, m_parentEntryIndex);
//		else
//			removeThis();
}

void IdmRootNode::viewThis(INodeView *nodeView, const QModelIndex &selected)
{
	addView(nodeView);
	nodeView->setNode(this, &m_proxy, &m_delegate);

	if (selected.isValid())
		nodeView->select(selected);
	else
		nodeView->select(rootIndex());
}

void IdmRootNode::viewChild(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<IdmItem*>(index.internalPointer())->isMenuItem())
		switch (static_cast<IdmMenuItem*>(index.internalPointer())->id())
		{
			case MainMenuItems::Create:
			{
				IdmEntity *ratingValue = m_storage.createEntity(tr("Rating value"), IdmEntity::Int);
				IdmEntity *tagNames = m_storage.createEntity(tr("Tag name"), IdmEntity::Int);
				IdmEntity *tag = m_storage.createEntity(tr("Tag"), IdmEntity::Composite);

				m_storage.addProperty(tag, tagNames);
				m_storage.addProperty(tag, ratingValue);
				break;
			}
			case MainMenuItems::Remove:
			{
				break;
			}
		}

//	if (static_cast<FolderNodeItem*>(index.internalPointer())->isRootItem())
//	{
//		removeView(nodeView);
//		static_cast<Node*>(Node::parent())->viewThis(nodeView, m_parentEntryIndex);
//		static_cast<Node*>(Node::parent())->refresh();
//	}
//	else
//		if (!static_cast<FolderNodeEntry*>(index.internalPointer())->isLocked())
//		{
//			FolderNodeEntry *entry = static_cast<FolderNodeEntry*>(index.internalPointer());
//			entry->refresh();
//
//			if (entry->exists())
//			{
//				Q_ASSERT(m_items.indexOf(entry) != Values::InvalidIndex);
//				Values::Value &value = m_items[m_items.indexOf(entry)];
//
//				if (value.node != 0)
//					value.node->setParentEntryIndex(idx);
//				else
//					if (value.node = createNode(*value.item, plugins))
//						value.node->setParentEntryIndex(idx);
//
//				if (value.node)
//				{
//					removeView(nodeView);
//					value.node->viewThis(nodeView, QModelIndex());
//					value.node->refresh();
//				}
//			}
//			else
//				removeEntry(index);
//		}
}

void IdmRootNode::viewChild(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins)
{
//	Values::size_type index = m_items.indexOf(*path);
//
//	if (index == Values::InvalidIndex)
//	{
//		Info info(absoluteFilePath(*path));
//
//		if (Node *node = createNode(info, plugins))
//		{
//			removeView(nodeView);
//
//			beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
//			m_items.add(Values::Value(new FolderNodeEntry(info), node));
//			endInsertRows();
//
//			if ((++path).atEnd())
//				node->viewThis(nodeView, QModelIndex());
//			else
//				node->viewChild(nodeView, path, plugins);
//		}
//		else
//			if (info.isFile())
//			{
//				FolderNodeItem *item;
//
//				beginInsertRows(QModelIndex(), m_items.size(), m_items.size());
//				m_items.add(Values::Value(item = new FolderNodeEntry(info)));
//				endInsertRows();
//
//				viewThis(nodeView, indexForFile(item, m_items.lastIndex()));
//			}
//			else
//				viewThis(nodeView, QModelIndex());
//	}
//	else
//	{
//		Values::Value &value = m_items[index];
//
//		if (value.node || (value.node = createNode(*value.item, plugins)))
//		{
//			removeView(nodeView);
//
//			if ((++path).atEnd())
//				value.node->viewThis(nodeView, QModelIndex());
//			else
//				value.node->viewChild(nodeView, path, plugins);
//		}
//		else
//			if (value.item->isFile())
//				viewThis(nodeView, indexForFile(value.item, index));
//			else
//				viewThis(nodeView, QModelIndex());
//	}
}

void IdmRootNode::viewAbsolute(INodeView *nodeView, const QString &absoluteFilePath, PluginsManager *plugins)
{
	Path path(absoluteFilePath);

	if (path.isAbsolute())
	{
		Node *node = this;
		removeView(nodeView);

		while (node->parent())
			node = static_cast<Node*>(node->parent());

		node->viewChild(nodeView, path.begin(), plugins);
	}
	else
		viewChild(nodeView, path.begin(), plugins);
}

void IdmRootNode::switchTo(Node *node, const QModelIndex &selected)
{
//	Node *child;
//
//	for (ViewSet::iterator it = m_view.begin(), end = m_view.end(); it != end; it = m_view.erase(it))
//		node->viewThis(*it, selected);
//
//	for (Values::size_type i = 0, size = m_items.size(); i < size; ++i)
//		if (child = m_items.at(i).node)
//			child->switchTo(node, selected);
}

QModelIndex IdmRootNode::rootIndex() const
{
	return QModelIndex();
}

void IdmRootNode::addView(INodeView *view)
{
	m_view.insert(view);
}

void IdmRootNode::removeView(INodeView *view)
{
	m_view.remove(view);
}

FILE_SYSTEM_NS_END
