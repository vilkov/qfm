#ifndef GROUPCONSTRAINTSITEM_H_
#define GROUPCONSTRAINTSITEM_H_

#include <QtCore/QList>
#include "queryconstraintsmodelitem.h"


IDM_PLUGIN_NS_BEGIN

class GroupConstraintsItem : public QueryConstraintsModelItem
{
public:
	typedef QList<QueryConstraintsModelItem*> Container;
	typedef Container::size_type              size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	GroupConstraintsItem(QueryConstraintsModelItem *parent = 0);
	virtual ~GroupConstraintsItem();

	/* QueryConstraintsModelItem */
	virtual bool isGroup() const;
	virtual QVariant data(qint32 column, qint32 role) const;

	size_type size() const { return m_items.size(); }

private:
	QVariant m_value;
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* GROUPCONSTRAINTSITEM_H_ */
