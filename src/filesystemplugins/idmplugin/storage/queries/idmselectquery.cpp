#include "idmselectquery.h"


IDM_PLUGIN_NS_BEGIN

Select::Select(IdmEntity *entity) :
	Query(entity)
{}

QByteArray Select::compile() const
{
	return QString::fromLatin1("select VALUE from ENTITY_").
			append(QString::number(entity()->id())).toUtf8();
}

IDM_PLUGIN_NS_END
