#ifndef IDMINODEQUERYRESULTS_H_
#define IDMINODEQUERYRESULTS_H_

#include <QtCore/QModelIndex>
#include "../../../storage/values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IQueryResultsUpdater
{
public:
	virtual ~IQueryResultsUpdater();

	virtual void add(const QModelIndex &index, const IdmCompositeEntityValue::List &values) = 0;
	virtual void remove(const QModelIndex &index, const IdmCompositeEntityValue::List &values) = 0;
};

IDM_PLUGIN_NS_END

#endif /* IDMINODEQUERYRESULTS_H_ */
