#include "idmrootnode.h"


FILE_SYSTEM_NS_BEGIN

IdmRootNode::IdmRootNode(Node *parent)
{

}

IdmRootNode::~IdmRootNode()
{

}

int IdmRootNode::rowCount(const QModelIndex &parent) const
{

}

int IdmRootNode::columnCount(const QModelIndex &parent) const
{

}

QVariant IdmRootNode::data(const QModelIndex &index, int role) const
{

}

Qt::ItemFlags IdmRootNode::flags(const QModelIndex &index) const
{

}

QVariant IdmRootNode::headerData(int section, Qt::Orientation orientation, int role) const
{

}

QModelIndex IdmRootNode::index(int row, int column, const QModelIndex &parent) const
{

}

QModelIndex IdmRootNode::parent(const QModelIndex &child) const
{

}

INode *IdmRootNode::root() const
{

}

int IdmRootNode::columnCount() const
{

}

IFileInfo *IdmRootNode::info(const QModelIndex &idx) const
{

}

IFileControl *IdmRootNode::createControl() const
{

}

IFileControl *IdmRootNode::createControl(const QModelIndex &idx, PluginsManager *plugins)
{

}

bool IdmRootNode::isDir() const
{

}

bool IdmRootNode::isFile() const
{

}

bool IdmRootNode::exists() const
{

}

QString IdmRootNode::fileName() const
{

}

QString IdmRootNode::absolutePath() const
{

}

QString IdmRootNode::absoluteFilePath() const
{

}

QString IdmRootNode::absoluteFilePath(const QString &fileName) const
{

}

QDateTime IdmRootNode::lastModified() const
{

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

}

void IdmRootNode::viewParent(INodeView *nodeView)
{

}

void IdmRootNode::viewThis(INodeView *nodeView, const QModelIndex &selected)
{

}

void IdmRootNode::viewChild(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins)
{

}

void IdmRootNode::viewChild(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins)
{

}

void IdmRootNode::viewAbsolute(INodeView *nodeView, const QString &absoluteFilePath, PluginsManager *plugins)
{

}

void IdmRootNode::switchTo(Node *node, const QModelIndex &selected)
{

}

FILE_SYSTEM_NS_END
