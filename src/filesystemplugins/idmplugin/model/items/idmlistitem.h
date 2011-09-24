#ifndef IDMLISTITEM_H_
#define IDMLISTITEM_H_

#include <QtCore/QList>
#include "idmitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmListItem : public IdmItem
{
public:
	typedef QList<IdmItem*>      Container;
	typedef Container::size_type size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	IdmListItem(IdmItem *parent = 0);
	virtual ~IdmListItem();

	/* IdmItem */
	virtual bool isList() const;

	IdmItem *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(IdmItem *item) const { return m_items.indexOf(item); }

protected:
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDMLISTITEM_H_ */
