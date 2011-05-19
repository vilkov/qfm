#include "filesystemrootnode.h"
#include <QtCore/QStringList>


FILE_SYSTEM_NS_BEGIN

RootNode::RootNode(PluginsManager *plugins) :
#ifdef Q_OS_WIN
	FolderNode(Info()),
#else
	FolderNode(QString::fromLatin1("/")),
#endif
	m_plugins(plugins)
{
	Q_ASSERT(m_plugins != 0);
}

//void RootNode::view(const QString &absoluteFilePath, INodeView *nodeView, QModelIndex &index)
//{
//	Node *node = this;
//	Node *parent;
//	QStringList list = absoluteFilePath.split(QChar('/'), QString::SkipEmptyParts);
//	QStringList::size_type idx = 0;
//
//	for (QStringList::size_type size = list.size(); idx < size && node; ++idx)
//	{
//		parent = node;
//		node = parent->subnode(list.at(idx), m_plugins);
//	}
//
//	if (node)
//		node->view(nodeView);
//	else
//	{
//		index = parent->indexFor(list.at(idx - 1));
//		parent->view(nodeView);
//	}
//}

FILE_SYSTEM_NS_END
