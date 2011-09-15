#ifndef IDMMENUITEM_H_
#define IDMMENUITEM_H_

#include <QtCore/QString>
#include <QtCore/QVariant>
#include "../../items/idmitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmMenuItem : public IdmItem
{
public:
	typedef unsigned int id_type;

public:
	IdmMenuItem(id_type id, const QString &label, const QString &toolTip, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;
	virtual bool isRoot() const;
	virtual bool isList() const;
	virtual bool isMenuItem() const;
	virtual bool isValueItem() const;
	virtual bool isEntityItem() const;

	id_type id() const { return m_id; }

private:
	id_type m_id;
	QVariant m_label;
	QVariant m_toolTip;
};

IDM_PLUGIN_NS_END

#endif /* IDMMENUITEM_H_ */
