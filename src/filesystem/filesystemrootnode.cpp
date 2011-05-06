#include "filesystemrootnode.h"
#include <QtCore/QStringList>
#include <QDebug>


FILE_SYSTEM_NS_BEGIN

RootNode::RootNode(PluginsManager *plugins) :
	FolderNode(QString::fromLatin1("/")),
	m_plugins(plugins)
{
	Q_ASSERT(m_plugins != 0);
}

void RootNode::view(const QString &absoluteFilePath, QAbstractItemView *itemView)
{
	bool ok;
	Node *tree = 0;
	Node *parent = this;
	QStringList list = absoluteFilePath.split(QChar('/'), QString::SkipEmptyParts);

	for (QStringList::size_type i = 0, size = list.size(); i < size && parent; ++i)
		parent = parent->node(list.at(i), m_plugins);

	if (parent)
		parent->view(itemView);
}

FILE_SYSTEM_NS_END
