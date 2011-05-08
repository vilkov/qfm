#include "m3unode.h"


M3uNode::M3uNode(const FileSystem::Info &info, Node *parent)
{

}

int M3uNode::rowCount(const QModelIndex &parent) const
{

}

int M3uNode::columnCount(const QModelIndex &parent) const
{

}

QVariant M3uNode::data(const QModelIndex &index, int role) const
{

}

Qt::ItemFlags M3uNode::flags(const QModelIndex &index) const
{

}

QVariant M3uNode::headerData(int section, Qt::Orientation orientation, int role) const
{

}

QModelIndex M3uNode::index(int row, int column, const QModelIndex &parent) const
{

}

QModelIndex M3uNode::parent(const QModelIndex &child) const
{

}

bool M3uNode::exists() const
{

}

QString M3uNode::fileName() const
{

}

QString M3uNode::absolutePath() const
{

}

QString M3uNode::absoluteFilePath() const
{

}

QDateTime M3uNode::lastModified() const
{

}

void M3uNode::refresh()
{

}

void M3uNode::remove(const QModelIndexList &index)
{

}

void M3uNode::copy(const QModelIndexList &index, Node *destination)
{

}

void M3uNode::move(const QModelIndexList &index, Node *destination)
{

}

void M3uNode::createFolder(const QString &name)
{

}

void M3uNode::createFile(const QString &name)
{

}

void M3uNode::update()
{

}

FileSystem::Node *M3uNode::subnode(const QModelIndex &idx, FileSystem::PluginsManager *plugins)
{

}

void M3uNode::remove(Node *subnode)
{

}

void M3uNode::view(QAbstractItemView *itemView)
{

}

QModelIndex M3uNode::indexFor(const QString &fileName)
{

}

QModelIndex M3uNode::rootIndex() const
{

}

bool M3uNode::isRootIndex(const QModelIndex &index) const
{

}

FileSystem::Node *M3uNode::node(const QString &fileName, FileSystem::PluginsManager *plugins)
{

}

