#ifndef STATICVALUELISTMODEL_H_
#define STATICVALUELISTMODEL_H_

#include "../../model/valuelistmodel.h"


IDM_PLUGIN_NS_BEGIN

class StaticValueListModel : public ValueListModel
{
public:
	StaticValueListModel(const IdmContainer &container, const Select &query, QObject *parent = 0);
};

IDM_PLUGIN_NS_END

#endif /* STATICVALUELISTMODEL_H_ */
