#include "arhnode.h"


ARH_PLUGIN_NS_BEGIN

ArhNode::ArhNode(const Info &info, Node *parent) :
	TasksNode(m_itemsContainer, parent),
	m_items(m_itemsContainer.m_container),
	m_info(info)
{}

FileTypeId ArhNode::id() const
{
	return m_info.id();
}

QIcon ArhNode::icon() const
{
	return m_info.icon();
}

QString ArhNode::name() const
{
	return m_info.name();
}

QString ArhNode::description() const
{
	return m_info.description();
}

bool ArhNode::isDir() const
{
	return m_info.isDir();
}

bool ArhNode::isFile() const
{
	return m_info.isFile();
}

bool ArhNode::isLink() const
{
	return m_info.isLink();
}

bool ArhNode::exists() const
{
	return m_info.exists();
}

qint64 ArhNode::fileSize() const
{
	return m_info.fileSize();
}

QString ArhNode::fileName() const
{
	return m_info.fileName();
}

QString ArhNode::absolutePath() const
{
	return m_info.absolutePath();
}

QString ArhNode::absoluteFilePath() const
{
	return m_info.absoluteFilePath();
}

QString ArhNode::absoluteFilePath(const QString &fileName) const
{
	return m_info.absoluteFilePath(fileName);
}

QDateTime ArhNode::lastModified() const
{
	return m_info.lastModified();
}

int ArhNode::permissions() const
{
	return m_info.permissions();
}

void ArhNode::refresh()
{

}

IFileInfo *ArhNode::info(const QModelIndex &idx) const
{
	return 0;
}

ICopyControl *ArhNode::createControl(INodeView *view) const
{
	return 0;
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
	return createIndex(0, 0, m_items.at(0));
}

QAbstractItemModel *ArhNode::proxyModel() const
{
	return const_cast<ArhNode*>(this);
}

QAbstractItemDelegate *ArhNode::itemDelegate() const
{
	return const_cast<ArhDelegate*>(&m_delegate);
}

const INodeView::MenuActionList &ArhNode::menuActions() const
{
	return m_actions;
}

Node *ArhNode::viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
}

Node *ArhNode::viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected)
{
	return 0;
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
