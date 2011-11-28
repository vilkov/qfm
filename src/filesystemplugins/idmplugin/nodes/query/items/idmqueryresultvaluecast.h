#ifndef IDMQUERYRESULTVALUECAST_H_
#define IDMQUERYRESULTVALUECAST_H_

#include "idmqueryresultpathvalueitem.h"
#include "idmqueryresultpropertyitem.h"
#include "idmqueryresultrootitem.h"


IDM_PLUGIN_NS_BEGIN

template <typename T> inline T *value_cast(void *item, T *to);

template <> inline QueryResultValueItem *value_cast(void *item, QueryResultValueItem *to)
{
	Q_UNUSED(to);

	if (static_cast<IdmItem*>(item)->isList())
		return 0;
	else
		return static_cast<QueryResultValueItem*>(item);
}

template <> inline QueryResultListItem *value_cast(void *item, QueryResultListItem *to)
{
	Q_UNUSED(to);

	if (static_cast<IdmItem*>(item)->isList())
		return static_cast<QueryResultListItem*>(item);
	else
		return 0;
}

template <> inline QueryResultPropertyItem *value_cast(void *item, QueryResultPropertyItem *to)
{
	Q_UNUSED(to);

	if (static_cast<IdmItem*>(item)->isList() &&
		static_cast<QueryResultListItem*>(item)->isProperty())
		return static_cast<QueryResultPropertyItem*>(item);
	else
		return 0;
}

template <> inline QueryResultPathValueItem *value_cast(void *item, QueryResultPathValueItem *to)
{
	Q_UNUSED(to);

	if (static_cast<IdmItem*>(item)->isList())
		return 0;
	else
		if (static_cast<QueryResultValueItem*>(item)->value()->entity()->type() == Database::Path)
			return static_cast<QueryResultPathValueItem*>(item);
		else
			return 0;
}

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTVALUECAST_H_ */
