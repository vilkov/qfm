#include "arhnode.h"


ARH_PLUGIN_NS_BEGIN

ArhNode::ArhNode(Node *parent) :
	TasksNode(m_itemsContainer, parent),
	m_items(m_itemsContainer.m_container)
{}

FileTypeId ArhNode::id() const
{

}

QIcon ArhNode::icon() const
{

}

QString ArhNode::name() const
{

}

QString ArhNode::description() const
{

}

bool ArhNode::isDir() const
{

}

bool ArhNode::isFile() const
{

}

bool ArhNode::isLink() const
{

}

bool ArhNode::exists() const
{

}

qint64 ArhNode::fileSize() const
{

}

QString ArhNode::fileName() const
{

}

QString ArhNode::absolutePath() const
{

}

QString ArhNode::absoluteFilePath() const
{

}

QString ArhNode::absoluteFilePath(const QString &fileName) const
{

}

QDateTime ArhNode::lastModified() const
{

}

int ArhNode::permissions() const
{

}

void ArhNode::refresh()
{

}

IFileInfo *ArhNode::info(const QModelIndex &idx) const
{

}

ICopyControl *ArhNode::createControl(INodeView *view) const
{

}

void ArhNode::menuAction(QAction *action, INodeView *view)
{

}

void ArhNode::createFile(const QModelIndex &index, INodeView *view)
{

}

void ArhNode::createDirectory(const QModelIndex &index, INodeView *view)
{

}

void ArhNode::rename(const QModelIndex &index, INodeView *view)
{

}

void ArhNode::rename(const QModelIndexList &list, INodeView *view)
{

}

void ArhNode::remove(const QModelIndexList &list, INodeView *view)
{

}

void ArhNode::cancel(const QModelIndexList &list, INodeView *view)
{

}

void ArhNode::calculateSize(const QModelIndexList &list, INodeView *view)
{

}

void ArhNode::pathToClipboard(const QModelIndexList &list, INodeView *view)
{

}

void ArhNode::copy(const INodeView *source, INodeView *destination)
{

}

void ArhNode::move(const INodeView *source, INodeView *destination)
{

}

QModelIndex ArhNode::rootIndex() const
{

}

QAbstractItemModel *ArhNode::proxyModel() const
{

}

QAbstractItemDelegate *ArhNode::itemDelegate() const
{

}

const INodeView::MenuActionList &ArhNode::menuActions() const
{

}

Node *ArhNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{

}

Node *ArhNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{

}

void ArhNode::nodeRemoved(Node *node)
{

}

void ArhNode::updateProgressEvent(TaskNodeItem::Base *item, quint64 progress, quint64 timeElapsed)
{

}

void ArhNode::completedProgressEvent(TaskNodeItem::Base *item, quint64 timeElapsed)
{

}

ArhNode::ItemsContainer::ItemsContainer()
{}

ArhNode::ItemsContainer::~ItemsContainer()
{
	qDeleteAll(m_container);
}

ArhNode::ItemsContainer::size_type ArhNode::ItemsContainer::size() const
{
	return m_container.size();
}

ArhNode::ItemsContainer::Item *ArhNode::ItemsContainer::at(size_type index) const
{
	return m_container.at(index);
}

ArhNode::ItemsContainer::size_type ArhNode::ItemsContainer::indexOf(Item *item) const
{
	return m_container.indexOf(item);
}

ARH_PLUGIN_NS_END
