#include "idmnodebase.h"
#include "idmnode.h"
#include "../items/idmroot.h"
#include "../items/idmmessage.h"
#include "../items/idmseparator.h"
#include "../gui/createentitydialog.h"
#include "../../../application.h"


FILE_SYSTEM_NS_BEGIN

IdmNodeBase::IdmNodeBase(IdmContainer *storage, const Info &info, Node *parent) :
	FolderNodeBase(info, parent),
	m_proxy(this),
	m_delegate(&m_proxy),
	m_container(storage)
{
	m_proxy.setDynamicSortFilter(true);
	m_proxy.setSourceModel(this);
}

IdmNodeBase::~IdmNodeBase()
{
	if (!m_items.isEmpty())
	{
		if (m_items.at(0)->isList())
			m_items.removeAt(0);

		qDeleteAll(m_items);
	}
}

int IdmNodeBase::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		if (static_cast<IdmItem*>(parent.internalPointer())->isList())
			return static_cast<IdmItemsList*>(parent.internalPointer())->size();
		else
			return 0;
	else
    	return m_items.size();
}

int IdmNodeBase::columnCount(const QModelIndex &parent) const
{
	return 2;
}

QVariant IdmNodeBase::data(const QModelIndex &index, int role) const
{
    if (index.isValid())
    	return static_cast<IdmItem*>(index.internalPointer())->data(index.column(), role);
    else
    	return m_items.at(index.row())->data(index.column(), role);
}

Qt::ItemFlags IdmNodeBase::flags(const QModelIndex &index) const
{
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant IdmNodeBase::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex IdmNodeBase::index(int row, int column, const QModelIndex &parent) const
{
	if (hasIndex(row, column, parent))
		if (parent.isValid() && static_cast<IdmItem*>(parent.internalPointer())->isList())
			return createIndex(row, column, static_cast<IdmItemsList*>(parent.internalPointer())->at(row));
		else
			return createIndex(row, column, m_items.at(row));
    else
        return QModelIndex();
}

QModelIndex IdmNodeBase::parent(const QModelIndex &child) const
{
    if (child.isValid())
		if (IdmItem *parent = static_cast<IdmItem*>(child.internalPointer())->parent())
			if (parent->parent())
				return createIndex(static_cast<IdmItemsList*>(parent->parent())->indexOf(parent), 0, parent);
			else
				return createIndex(m_items.indexOf(parent), 0, parent);

    return QModelIndex();
}

IFileInfo *IdmNodeBase::info(const QModelIndex &idx) const
{
	return 0;
}

IFileControl *IdmNodeBase::createControl(const QModelIndex &idx, PluginsManager *plugins)
{
	return 0;
}

void IdmNodeBase::remove(const QModelIndexList &list)
{

}

void IdmNodeBase::cancel(const QModelIndexList &list)
{

}

void IdmNodeBase::calculateSize(const QModelIndexList &list)
{

}

void IdmNodeBase::pathToClipboard(const QModelIndexList &list)
{

}

void IdmNodeBase::copy(const QModelIndexList &list, INode *destination)
{

}

void IdmNodeBase::move(const QModelIndexList &list, INode *destination)
{

}

void IdmNodeBase::switchTo(Node *node, const QModelIndex &selected)
{
	FolderNodeBase::switchTo(node, selected);

//	Node *child;
//	for (Values::size_type i = 0, size = m_items.size(); i < size; ++i)
//		if (child = m_items.at(i).node)
//			child->switchTo(node, selected);
}

QModelIndex IdmNodeBase::rootIndex() const
{
	return QModelIndex();
}

Node *IdmNodeBase::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	QModelIndex index = m_proxy.mapToSource(idx);

	if (static_cast<IdmItem*>(index.internalPointer())->isRoot())
		return static_cast<Node*>(Node::parent());
	else
		if (static_cast<IdmItem*>(index.internalPointer())->isMenuItem())
			switch (static_cast<IdmMenuItem*>(index.internalPointer())->id())
			{
				case IdmContainer::Create:
				{
					CreateEntityDialog dialog(m_container, QString(), &Application::instance()->mainWindow());

					if (dialog.exec() == CreateEntityDialog::Accepted)
					{
						IdmEntity *tagRating = m_container->createEntity(tr("Tag rating"), IdmEntity::Rating);
						IdmEntity *tagName = m_container->createEntity(tr("Tag name"), IdmEntity::Int);

						IdmEntity *tag = m_container->createEntity(tr("Tag"), IdmEntity::Composite);
						m_container->addProperty(tag, tagName);
						m_container->addProperty(tag, tagRating);
					}

					break;
				}
				case IdmContainer::Remove:
				{
					break;
				}
			}

	return 0;
}

Node *IdmNodeBase::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
}

UpdatesList::Map IdmNodeBase::updateFilesMap() const
{
	return UpdatesList::Map();
}

void IdmNodeBase::updateFilesEvent(const UpdatesList &updates)
{

}

void IdmNodeBase::scanForSizeEvent(bool canceled, PScopedPointer<FileSystemList> &entries)
{

}

void IdmNodeBase::scanForCopyEvent(bool canceled, PScopedPointer<FileSystemList> &entries, PScopedPointer<IFileControl> &destination, bool move)
{

}

void IdmNodeBase::scanForRemoveEvent(bool canceled, PScopedPointer<FileSystemList> &entries)
{

}

void IdmNodeBase::performCopyEvent(bool canceled, PScopedPointer<FileSystemList> &entries, bool move)
{

}

void IdmNodeBase::performRemoveEvent(PScopedPointer<FileSystemList> &entries)
{

}

void IdmNodeBase::updateProgressEvent(const QString &fileName, quint64 progress, quint64 timeElapsed)
{

}

void IdmNodeBase::completedProgressEvent(const QString &fileName, quint64 timeElapsed)
{

}

FILE_SYSTEM_NS_END
