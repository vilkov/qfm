#ifndef QUERYCONSTRAINTSMODEL_H_
#define QUERYCONSTRAINTSMODEL_H_

#include "../../../model/idmmodel.h"


IDM_PLUGIN_NS_BEGIN

class QueryConstraintsModel : public IdmModel
{
public:
	QueryConstraintsModel(QObject *parent = 0);

	void add(const QModelIndex &index);
	void remove(const QModelIndex &index);
};

IDM_PLUGIN_NS_END

#endif /* QUERYCONSTRAINTSMODEL_H_ */
