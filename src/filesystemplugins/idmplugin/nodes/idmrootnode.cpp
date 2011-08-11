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

void IdmRootNode::switchTo(Node *node, const QModelIndex &selected)
{
	Node::switchTo(node, selected);

//	Node *child;
//	for (Values::size_type i = 0, size = m_items.size(); i < size; ++i)
//		if (child = m_items.at(i).node)
//			child->switchTo(node, selected);
}

QModelIndex IdmRootNode::rootIndex() const
{
	return QModelIndex();
}

Node *IdmRootNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<IdmItem*>(index.internalPointer())->isRoot())
		return static_cast<Node*>(Node::parent());
	else
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

	return 0;
}

Node *IdmRootNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
}

FILE_SYSTEM_NS_END
