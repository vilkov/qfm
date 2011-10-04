#include "staticvaluelistmodel.h"


IDM_PLUGIN_NS_BEGIN

StaticValueListModel::StaticValueListModel(const IdmContainer &container, const Select &query, QObject *parent) :
	ValueListModel(container, query, parent)
{}

IDM_PLUGIN_NS_END
