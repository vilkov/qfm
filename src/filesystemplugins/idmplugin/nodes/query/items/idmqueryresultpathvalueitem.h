#ifndef IDMQUERYRESULTPATHVALUEITEM_H_
#define IDMQUERYRESULTPATHVALUEITEM_H_

#include "idmqueryresultvalueitem.h"
#include "../../../../../filesystem/info/filesysteminfo.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultPathValueItem : public QueryResultValueItem
{
public:
	QueryResultPathValueItem(IdmEntityValue *value, Base *parent);

	const Info &info() const { return m_info; }
	Info &info() { return m_info; }

	void update() { m_info = value()->value().toString(); }

private:
	Info m_info;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTPATHVALUEITEM_H_ */
