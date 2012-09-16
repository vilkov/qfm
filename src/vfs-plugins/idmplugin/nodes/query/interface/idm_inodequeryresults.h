#ifndef IDM_INODEQUERYRESULTS_H_
#define IDM_INODEQUERYRESULTS_H_

#include <QtCore/QModelIndex>
#include "../../../storage/values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IQueryResultsUpdater
{
public:
	virtual ~IQueryResultsUpdater();

	virtual void add(const QModelIndex &index, const IdmCompositeEntityValue::List &values) = 0;
	virtual void remove(const QModelIndex &index, const IdmCompositeEntityValue::List &values) = 0;
	virtual void refresh(const QModelIndex &index) = 0;
};

IDM_PLUGIN_NS_END

#endif /* IDM_INODEQUERYRESULTS_H_ */
