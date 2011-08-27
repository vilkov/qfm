#ifndef IDMITEM_H_
#define IDMITEM_H_

#include <QtCore/QVariant>
#include "../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class IdmItem
{
public:
	IdmItem(IdmItem *parent);
	virtual ~IdmItem();

	IdmItem *parent() const { return m_parent; }

	virtual QVariant data(qint32 column, qint32 role) const = 0;
	virtual bool isRoot() const = 0;
	virtual bool isList() const = 0;
	virtual bool isMenuItem() const = 0;
	virtual bool isEntityItem() const = 0;

private:
	IdmItem *m_parent;
};

IDM_PLUGIN_NS_END

#endif /* IDMITEM_H_ */
