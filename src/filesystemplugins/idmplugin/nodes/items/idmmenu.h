#ifndef IDMMENU_H_
#define IDMMENU_H_

#include <QtCore/QList>
#include <QtCore/QString>
#include "idmitem.h"
#include "mainmenu/idmmenuitem.h"


FILE_SYSTEM_NS_BEGIN

class IdmMenu : public IdmItem
{
public:
	typedef QList<IdmMenuItem*>   value_type;
	typedef value_type::size_type size_type;

public:
	IdmMenu(const QString &label, const QString &toolTip, IdmItem *parent = 0) :
		IdmItem(parent),
		m_label(label),
		m_toolTip(toolTip)
	{}
	virtual ~IdmMenu()
	{
		qDeleteAll(m_items);
	}

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
	virtual bool isRoot() const { return false; }
	virtual bool isMenu() const { return true; }
	virtual bool isMenuItem() const { return false; }

	IdmMenuItem *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }

	void add(IdmMenuItem::id_type id, const QString &label, const QString &toolTip)
	{
		m_items.push_back(new IdmMenuItem(id, label, toolTip, this));
	}

private:
	QVariant m_label;
	QVariant m_toolTip;
	value_type m_items;
};

FILE_SYSTEM_NS_END

#endif /* IDMMENU_H_ */
