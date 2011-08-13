#ifndef IDMMENU_H_
#define IDMMENU_H_

#include "idmmenuitem.h"
#include "../../items/idmitemslist.h"


FILE_SYSTEM_NS_BEGIN

class IdmMenu : public IdmItemsList
{
public:
	IdmMenu(const QString &label, const QString &toolTip, IdmItem *parent = 0) :
		IdmItemsList(parent),
		m_label(label),
		m_toolTip(toolTip)
	{}

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const
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
						return Qt::AlignCenter;
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
						return Qt::AlignCenter;
					case Qt::ToolTipRole:
						return m_toolTip;
				}
				break;
			}
		}

		return QVariant();
	}

	IdmMenu *add(const QString &label, const QString &toolTip)
	{
		IdmMenu *res;
		m_items.push_back(res = new IdmMenu(label, toolTip, this));
		return res;
	}
	void add(IdmMenuItem::id_type id, const QString &label, const QString &toolTip)
	{
		m_items.push_back(new IdmMenuItem(id, label, toolTip, this));
	}

private:
	QVariant m_label;
	QVariant m_toolTip;
};

FILE_SYSTEM_NS_END

#endif /* IDMMENU_H_ */
