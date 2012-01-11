#include "arcnodeentryitem.h"
#include "../../../../application.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeEntryItem::ArcNodeEntryItem(const QString &fileName, Base *parent) :
	ArcNodeItem(parent),
	m_fileName(fileName),
	m_typeInfo(Application::desktopEnvironment()->fileTypeInfoFromFileName(m_fileName, false, 16))
{}

QVariant ArcNodeEntryItem::data(qint32 column, qint32 role) const
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

bool ArcNodeEntryItem::isRoot()
{
	return false;
}

bool ArcNodeEntryItem::isDir()
{
	return false;
}

ARC_PLUGIN_NS_END
