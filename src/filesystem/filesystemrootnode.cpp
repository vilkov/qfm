#include "filesystemrootnode.h"
#include <QtCore/QStringList>


FILE_SYSTEM_NS_BEGIN

RootNode::RootNode(PluginsManager *plugins) :
	FolderNode(QString::fromLatin1("/")),
	m_plugins(plugins)
{
	Q_ASSERT(m_plugins != 0);
}

void RootNode::view(const QString &absoluteFilePath, QAbstractItemView *itemView, QModelIndex &index)
{
	Node *node = this;
	Node *parent;
	QStringList list = absoluteFilePath.split(QChar('/'), QString::SkipEmptyParts);
	QStringList::size_type idx = 0;

	for (QStringList::size_type size = list.size(); idx < size && node; ++idx)
	{
		parent = node;
		node = parent->node(list.at(idx), m_plugins);
	}

	if (node)
		node->view(itemView);
	else
	{
		index = parent->indexFor(list.at(idx - 1));
		parent->view(itemView);
	}
}

FILE_SYSTEM_NS_END
