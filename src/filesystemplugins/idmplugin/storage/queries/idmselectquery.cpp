#include "idmselectquery.h"


IDM_PLUGIN_NS_BEGIN

Select::Select(IdmEntity *entity) :
	Query(entity)
{}

Select::Select(IdmEntity *entity, BaseConstraint *where) :
	Query(entity),
	m_where(where)
{}

QByteArray Select::compile() const
{
	Format format;
	QString joinedFields;
	QString selectedFields = format.select(entity());

	for (IdmEntity::size_type i = 0, size = entity()->size(); i < size; ++i)
		join(format, selectedFields, joinedFields, entity(), entity()->at(i).entity);

	return format.complete(entity()->id(), selectedFields, joinedFields).toUtf8();
}

Select::Format::Format() :
	format(QString::fromLatin1("%1, %2, ")),
	format2(QString::fromLatin1("%1, ")),
	idField(QString::fromLatin1("ENTITY_%1.ID")),
	valueField(QString::fromLatin1("ENTITY_%1.VALUE"))
{}

QString Select::Format::select(IdmEntity *entity) const
{
	if (entity->type() == Database::Composite)
		return QString(format2).
				arg(QString(idField).arg(entity->id()));
	else
		return QString(format).
				arg(QString(idField).arg(entity->id())).
				arg(QString(valueField).arg(entity->id()));
}

QString Select::Format::join(Database::id_type entity, Database::id_type property) const
{
	return QString::fromLatin1("left join ENTITY_%1_PROPERTY_%2 on ENTITY_%1_PROPERTY_%2.ENTITY_VALUE_ID = ENTITY_%1.ID "
							   "left join ENTITY_%2 on ENTITY_%2.ID = ENTITY_%1_PROPERTY_%2.PROPERTY_VALUE_ID ").
			arg(QString::number(entity)).
			arg(QString::number(property));
}

QString Select::Format::complete(Database::id_type entity, QString &selectedFields, QString &joinedFields) const
{
	selectedFields.chop(2);

	if (joinedFields.isEmpty())
		return QString::fromLatin1("select ").
				append(selectedFields).
				append(QString::fromLatin1(" from ENTITY_%1").arg(QString::number(entity)));
	else
	{
		joinedFields.chop(1);

		return QString::fromLatin1("select ").
				append(selectedFields).
				append(QString::fromLatin1(" from ENTITY_%1 ").arg(QString::number(entity))).
				append(joinedFields);
	}
}

void Select::join(const Format &format, QString &selectedFields, QString &joinedFields, IdmEntity *entity, IdmEntity *property) const
{
	selectedFields += format.select(property);
	joinedFields += format.join(entity->id(), property->id());

	for (IdmEntity::size_type i = 0, size = property->size(); i < size; ++i)
		join(format, selectedFields, joinedFields, property, property->at(i).entity);
}

IDM_PLUGIN_NS_END
