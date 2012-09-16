#ifndef IDM_LISTITEM_H_
#define IDM_LISTITEM_H_

#include "idm_item.h"


IDM_PLUGIN_NS_BEGIN

class IdmListItem : public IdmItem
{
public:
	typedef QList<IdmItem *> Container;

public:
	IdmListItem(Base *parent);

	/* Base */
	virtual Base *at(size_type index) const;
	virtual size_type size() const;
	virtual size_type indexOf(Base *item) const;

protected:
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDM_LISTITEM_H_ */
