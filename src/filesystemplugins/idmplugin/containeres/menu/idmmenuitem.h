#ifndef IDMMENUITEM_H_
#define IDMMENUITEM_H_

#include <QtCore/QString>
#include <QtCore/QVariant>
#include "../../items/idmitem.h"


FILE_SYSTEM_NS_BEGIN

class IdmMenuItem : public IdmItem
{
public:
	typedef unsigned int id_type;

public:
	IdmMenuItem(id_type id, const QString &label, const QString &toolTip, IdmItem *parent = 0) :
		IdmItem(parent),
		m_id(id),
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
	virtual bool isRoot() const { return false; }
	virtual bool isList() const { return false; }
	virtual bool isMenuItem() const { return true; }
	virtual bool isEntity() const { return false; }

	id_type id() const { return m_id; }

private:
	id_type m_id;
	QVariant m_label;
	QVariant m_toolTip;
};

FILE_SYSTEM_NS_END

#endif /* IDMMENUITEM_H_ */
