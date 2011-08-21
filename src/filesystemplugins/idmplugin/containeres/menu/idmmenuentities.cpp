#include "idmmenuentities.h"


FILE_SYSTEM_NS_BEGIN

IdmMenuEntities::IdmMenuEntities(const QString &label, const QString &toolTip, IdmItem *parent) :
	IdmMenuEntitiesItem(0, parent),
	m_label(label),
	m_toolTip(toolTip)
{}

QVariant IdmMenuEntities::data(qint32 column, qint32 role) const
{
	switch (column)
	{
		case 0:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return m_label;
//					case Qt::DecorationRole:
//						if (m_locked)
//							return Application::style()->standardIcon(QStyle::SP_BrowserReload);
//						else
//							return m_info.icon();
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
				case Qt::ToolTipRole:
					return m_toolTip;
			}
			break;
		}
		case 1:
		{
			switch (role)
			{
				case Qt::EditRole:
				case Qt::DisplayRole:
					return m_toolTip;
//					case Qt::DecorationRole:
//						if (m_locked)
//							return Application::style()->standardIcon(QStyle::SP_BrowserReload);
//						else
//							return m_info.icon();
				case Qt::TextAlignmentRole:
					return Qt::AlignLeft;
				case Qt::ToolTipRole:
					return m_toolTip;
			}
			break;
		}
	}

	return QVariant();
}

bool IdmMenuEntities::isEntityItem() const
{
	return false;
}

FILE_SYSTEM_NS_END
