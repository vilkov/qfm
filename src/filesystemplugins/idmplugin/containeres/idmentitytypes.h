#ifndef IDMENTITYTYPES_H_
#define IDMENTITYTYPES_H_

#include <QtCore/QMap>
#include "../storage/entities/idmentity.h"


FILE_SYSTEM_NS_BEGIN

struct IdmEntityTypeDescription
{
	IdmEntityTypeDescription()
	{}
	IdmEntityTypeDescription(const QString &label, const QString &description) :
		label(label),
		description(description)
	{}

	QString label;
	QString description;
};

typedef QMap<IdmEntity::Type, IdmEntityTypeDescription> IdmEntityTypes;

FILE_SYSTEM_NS_END

#endif /* IDMENTITYTYPES_H_ */
