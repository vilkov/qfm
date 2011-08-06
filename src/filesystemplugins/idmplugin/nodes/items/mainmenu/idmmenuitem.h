#ifndef IDMMENUITEM_H_
#define IDMMENUITEM_H_

#include <QtCore/QString>
#include "../idmitem.h"


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
		if (column == 0 && role == Qt::DisplayRole)
			return m_label;
		else
			return QVariant();
	}
	virtual bool isRoot() const { return false; }
	virtual bool isMenu() const { return false; }
	virtual bool isMenuItem() const { return true; }

	id_type id() const { return m_id; }

private:
	id_type m_id;
	QVariant m_label;
	QVariant m_toolTip;
};

FILE_SYSTEM_NS_END

#endif /* IDMMENUITEM_H_ */
