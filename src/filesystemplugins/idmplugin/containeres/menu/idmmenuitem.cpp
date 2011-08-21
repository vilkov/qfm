#include "idmmenuitem.h"


FILE_SYSTEM_NS_BEGIN

IdmMenuItem::IdmMenuItem(id_type id, const QString &label, const QString &toolTip, IdmItem *parent) :
	IdmItem(parent),
	m_id(id),
	m_label(label),
	m_toolTip(toolTip)
{}

QVariant IdmMenuItem::data(qint32 column, qint32 role) const
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

bool IdmMenuItem::isRoot() const
{
	return false;
}

bool IdmMenuItem::isList() const
{
	return false;
}

bool IdmMenuItem::isMenuItem() const
{
	return true;
}

bool IdmMenuItem::isEntityItem() const
{
	return false;
}

FILE_SYSTEM_NS_END
