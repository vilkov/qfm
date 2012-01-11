#include "arcnodedirentryitem.h"
#include "../../../../application.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeDirEntryItem::ArcNodeDirEntryItem(const QString &fileName, Base *parent) :
	ArcNodeListItem(parent),
	m_fileName(fileName),
	m_typeInfo(Application::desktopEnvironment()->fileTypeInfoFromFileName(m_fileName, true, 16))
{}

QVariant ArcNodeDirEntryItem::data(qint32 column, qint32 role) const
{
	switch (role)
	{
		case Qt::DisplayRole:
			return m_fileName;

		case Qt::DecorationRole:
			if (isLocked())
				return lockIcon();
			else
				return m_typeInfo.icon;

		case Qt::TextAlignmentRole:
			return Qt::AlignLeft;

		case Qt::ToolTipRole:
			if (isLocked())
				return lockReason();
			else
				return QVariant();

		default:
			return QVariant();
	}
}

bool ArcNodeDirEntryItem::isRoot()
{
	return false;
}

bool ArcNodeDirEntryItem::isDir()
{
	return true;
}

ARC_PLUGIN_NS_END
