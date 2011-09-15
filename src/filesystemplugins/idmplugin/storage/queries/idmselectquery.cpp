#include "idmselectquery.h"


IDM_PLUGIN_NS_BEGIN

Select::Select(IdmEntity *entity) :
	Query(entity)
{}

QByteArray Select::compile() const
{
	return QByteArray("select VALUE from ENTITY_").append(entity()->id());
}

IDM_PLUGIN_NS_END
