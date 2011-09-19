#ifndef IDMSELECTQUERY_H_
#define IDMSELECTQUERY_H_

#include <QtCore/QMap>
#include "idmquery.h"


IDM_PLUGIN_NS_BEGIN

class Select : public Query
{
public:
	enum Operators
	{
		Less,
		Greater,
		Equal,
		Like
	};
	struct Condition
	{
		Condition()
		{}
		Condition(Operators op, const QVariant &value) :
			op(op),
			value(value)
		{}

		Operators op;
		QVariant value;
	};
	typedef QMap<IdmEntity*, Condition> Map;

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
