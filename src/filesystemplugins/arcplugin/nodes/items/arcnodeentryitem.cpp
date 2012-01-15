#include "arcnodeentryitem.h"
#include "../../../../filesystem/tools/filesystemcommontools.h"
#include "../../../../application.h"


ARC_PLUGIN_NS_BEGIN

ArcNodeEntryItem::ArcNodeEntryItem(const QString &fileName, qint64 extractedSize, const QDateTime &lastModified, Base *parent) :
	ArcNodeItem(parent),
	m_fileName(fileName),
	m_extractedSize(extractedSize),
	m_lastModified(lastModified),
	m_typeInfo(Application::desktopEnvironment()->fileTypeInfoFromFileName(m_fileName, false, 16))
{}

QVariant ArcNodeEntryItem::data(qint32 column, qint32 role) const
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

bool ArcNodeEntryItem::isRoot() const
{
	return false;
}

bool ArcNodeEntryItem::isDir() const
{
	return false;
}

ARC_PLUGIN_NS_END
