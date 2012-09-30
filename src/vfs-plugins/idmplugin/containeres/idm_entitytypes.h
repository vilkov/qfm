#ifndef IDM_ENTITYTYPES_H_
#define IDM_ENTITYTYPES_H_

#include <QtCore/QMap>
#include "../storage/entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

struct EntityTypeDescription
{
	EntityTypeDescription()
	{}
	EntityTypeDescription(const QString &label, const QString &description) :
		label(label),
		description(description)
	{}

	QString label;
	QString description;
};

typedef QMap<Entity::Type, EntityTypeDescription> EntityTypes;

IDM_PLUGIN_NS_END

#endif /* IDM_ENTITYTYPES_H_ */
