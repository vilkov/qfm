#ifndef IDMSELECTQUERY_H_
#define IDMSELECTQUERY_H_

#include <QtCore/QMap>
#include "idmquery.h"


IDM_PLUGIN_NS_BEGIN

class Select : public Query
{
public:
	struct Constraint
	{
		enum Operator
		{
			Less,
			LessEqual,
			Greater,
			GreaterEqual,
			Equal,
			Like
		};

		Constraint() :
			property(0),
			op(Equal),
			value(),
			id(IdmEntity::InvalidId)
		{}
		Constraint(IdmEntity *property, Operator op, const QVariant &value) :
			property(property),
			op(op),
			value(value),
			id(IdmEntity::InvalidId)
		{}
		Constraint(IdmEntity *property, Operator op, IdmEntity::id_type id) :
			property(property),
			op(op),
			id(id)
		{}

		IdmEntity *property;
		Operator op;
		QVariant value;
		IdmEntity::id_type id;
	};
	typedef QMap<IdmEntity*, Constraint> Map;

public:
	Select(IdmEntity *entity);

	virtual QByteArray compile() const;

private:
	struct Format
	{
		Format();

		QString select(Database::id_type entity) const;
		QString select(Database::id_type entity, QString &indexField) const;
		QString join(Database::id_type entity, Database::id_type property) const;
		QString complete(Database::id_type entity, QString &selectedFields, QString &joinedFields) const;

		QString format;
		QString idField;
		QString valueField;
	};

	void join(const Format &format, QString &selectedFields, QString &joinedFields, IdmEntity *entity, IdmEntity *property) const;

private:
	Map m_where;
};

IDM_PLUGIN_NS_END

#endif /* IDMSELECTQUERY_H_ */
