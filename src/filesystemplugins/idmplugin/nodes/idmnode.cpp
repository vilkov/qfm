#include "idmnode.h"
#include "items/idmmenu.h"
#include "items/idmroot.h"
#include "items/idmmessage.h"
#include "items/idmseparator.h"


FILE_SYSTEM_NS_BEGIN

IdmNode::IdmNode(const QFileInfo &storage, Node *parent) :
	FolderNodeBase(storage.absolutePath(), parent)
{
	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);
}

int IdmNode::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		if (static_cast<IdmItem*>(parent.internalPointer())->isList())
			return static_cast<IdmItemsList*>(parent.internalPointer())->size();
		else
			return 0;
	else
    	return m_items.size();
}

int IdmNode::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant IdmNode::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<IdmItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_items.at(index.row())->data(index.column(), role);
}

Qt::ItemFlags IdmNode::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant IdmNode::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex IdmNode::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		if (parent.isValid() && static_cast<IdmItem*>(parent.internalPointer())->isList())
			return createIndex(row, column, static_cast<IdmItemsList*>(parent.internalPointer())->at(row));
		else
			return createIndex(row, column, m_items.at(row));
    else
        return QModelIndex();
}

QModelIndex IdmNode::parent(const QModelIndex &child) const
{
    if (child.isValid())
		if (IdmItem *parent = static_cast<IdmItem*>(child.internalPointer())->parent())
			if (parent->parent())
				return createIndex(static_cast<IdmItemsList*>(parent->parent())->indexOf(parent), 0, parent);
			else
				return createIndex(m_items.indexOf(parent), 0, parent);

    return QModelIndex();
}

IFileInfo *IdmNode::info(const QModelIndex &idx) const
{
	return 0;
}

IFileControl *IdmNode::createControl(const QModelIndex &idx, PluginsManager *plugins)
{
	return 0;
}

void IdmNode::remove(const QModelIndexList &list)
{

}

void IdmNode::cancel(const QModelIndexList &list)
{

}

void IdmNode::calculateSize(const QModelIndexList &list)
{

}

void IdmNode::pathToClipboard(const QModelIndexList &list)
{

}

void IdmNode::copy(const QModelIndexList &list, INode *destination)
{

}

void IdmNode::move(const QModelIndexList &list, INode *destination)
{

}

void IdmNode::switchTo(Node *node, const QModelIndex &selected)
{
	FolderNodeBase::switchTo(node, selected);

//	Node *child;
//	for (Values::size_type i = 0, size = m_items.size(); i < size; ++i)
//		if (child = m_items.at(i).node)
//			child->switchTo(node, selected);
}

QModelIndex IdmNode::rootIndex() const
{
	return QModelIndex();
}

Node *IdmNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
//	QModelIndex index = m_proxy.mapToSource(idx);
//
//	if (static_cast<IdmItem*>(index.internalPointer())->isRoot())
//		return static_cast<Node*>(Node::parent());
//	else
//		if (static_cast<IdmItem*>(index.internalPointer())->isMenuItem())
//			switch (static_cast<IdmMenuItem*>(index.internalPointer())->id())
//			{
//				case MainMenuItems::Create:
//				{
//					IdmEntity *ratingValue = m_storage.createEntity(tr("Rating value"), IdmEntity::Int);
//					IdmEntity *tagNames = m_storage.createEntity(tr("Tag name"), IdmEntity::Int);
//					IdmEntity *tag = m_storage.createEntity(tr("Tag"), IdmEntity::Composite);
//
//					m_storage.addProperty(tag, tagNames);
//					m_storage.addProperty(tag, ratingValue);
//					break;
//				}
//				case MainMenuItems::Remove:
//				{
//					break;
//				}
//			}

	return 0;
}

Node *IdmNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
}

UpdatesList::Map IdmNode::updateFilesMap() const
{
	return UpdatesList::Map();
}

void IdmNode::updateFilesEvent(const UpdatesList &updates)
{

}

void IdmNode::scanForSizeEvent(bool canceled, PScopedPointer<FileSystemList> &entries)
{

}

void IdmNode::scanForCopyEvent(bool canceled, PScopedPointer<FileSystemList> &entries, PScopedPointer<IFileControl> &destination, bool move)
{

}

void IdmNode::scanForRemoveEvent(bool canceled, PScopedPointer<FileSystemList> &entries)
{

}

void IdmNode::performCopyEvent(bool canceled, PScopedPointer<FileSystemList> &entries, bool move)
{

}

void IdmNode::performRemoveEvent(PScopedPointer<FileSystemList> &entries)
{

}

void IdmNode::updateProgressEvent(const QString &fileName, quint64 progress, quint64 timeElapsed)
{

}

void IdmNode::completedProgressEvent(const QString &fileName, quint64 timeElapsed)
{

}

FILE_SYSTEM_NS_END
