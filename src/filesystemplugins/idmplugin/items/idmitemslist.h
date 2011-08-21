#ifndef IDMITEMSLIST_H_
#define IDMITEMSLIST_H_

#include <QtCore/QList>
#include "idmitem.h"


FILE_SYSTEM_NS_BEGIN

class IdmItemsList : public IdmItem
{
public:
	typedef QList<IdmItem*>       value_type;
	typedef value_type::size_type size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	IdmItemsList(IdmItem *parent = 0);
	virtual ~IdmItemsList();

	/* IdmItem */
	virtual bool isRoot() const;
	virtual bool isList() const;
	virtual bool isMenuItem() const;
	virtual bool isEntityItem() const;

	IdmItem *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(IdmItem *item) const { return m_items.indexOf(item); }

protected:
	value_type m_items;
};

FILE_SYSTEM_NS_END

#endif /* IDMITEMSLIST_H_ */
