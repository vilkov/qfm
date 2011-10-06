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
	QString selectedFields = format.select(entity()->id());

	for (IdmEntity::size_type i = 0, size = entity()->size(); i < size; ++i)
		join(format, selectedFields, joinedFields, entity(), entity()->at(i).entity);

	return format.complete(entity()->id(), selectedFields, joinedFields).toUtf8();
}

Select::Format::Format() :
	format(QString::fromLatin1("%1, %2, ")),
	idField(QString::fromLatin1("ENTITY_%1.ID")),
	valueField(QString::fromLatin1("ENTITY_%1.VALUE"))
{}

QString Select::Format::select(Database::id_type entity) const
{
	return QString(format).
			arg(QString(idField).arg(entity)).
			arg(QString(valueField).arg(entity));
}

QString Select::Format::select(Database::id_type entity, QString &indexField) const
{
	return QString(format).
			arg(QString(idField).arg(entity)).
			arg(indexField = QString(valueField).arg(entity));
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
	selectedFields += format.select(property->id());
	joinedFields += format.join(entity->id(), property->id());

	for (IdmEntity::size_type i = 0, size = property->size(); i < size; ++i)
		join(format, selectedFields, joinedFields, property, property->at(i).entity);
}

IDM_PLUGIN_NS_END
