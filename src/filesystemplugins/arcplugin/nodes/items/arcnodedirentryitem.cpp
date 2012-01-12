#include "arcnodedirentryitem.h"
#include "../../../../filesystem/tools/filesystemcommontools.h"
#include "../../../../application.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeDirEntryItem::ArcNodeDirEntryItem(const QString &fileName, const QDateTime &lastModified, Base *parent) :
	ArcNodeListItem(parent),
	m_fileName(fileName),
	m_extractedSize(0),
	m_lastModified(lastModified),
	m_typeInfo(Application::desktopEnvironment()->fileTypeInfoFromFileName(m_fileName, true, 16))
{}

QVariant ArcNodeDirEntryItem::data(qint32 column, qint32 role) const
{
	switch (column)
	{
		case 0:
		{
			switch (role)
			{
				case Qt::EditRole:
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
						break;
			}
			break;
		}
		case 1:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return Tools::humanReadableSize(m_extractedSize);

				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;

				case Qt::ToolTipRole:
					return m_typeInfo.name;
			}
			break;
		}
		case 2:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return m_lastModified;

				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
			}
			break;
		}
	}

	return QVariant();
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
