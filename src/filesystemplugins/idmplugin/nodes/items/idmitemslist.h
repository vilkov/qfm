#ifndef IDMITEMSLIST_H_
#define IDMITEMSLIST_H_

#include <QtCore/QList>
#include <QtCore/QString>
#include "idmitem.h"


FILE_SYSTEM_NS_BEGIN

class IdmItemsList : public IdmItem
{
public:
	typedef QList<IdmItem*>       value_type;
	typedef value_type::size_type size_type;

public:
	IdmItemsList(IdmItem *parent = 0) :
		IdmItem(parent)
	{}
	virtual ~IdmItemsList()
	{
		qDeleteAll(m_items);
	}

	virtual bool isRoot() const { return false; }
	virtual bool isList() const { return true; }
	virtual bool isMenuItem() const { return false; }

	IdmItem *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(IdmItem *item) const { return m_items.indexOf(item); }

protected:
	value_type m_items;
};

FILE_SYSTEM_NS_END

#endif /* IDMITEMSLIST_H_ */
