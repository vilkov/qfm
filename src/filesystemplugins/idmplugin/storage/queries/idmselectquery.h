#ifndef IDMSELECTQUERY_H_
#define IDMSELECTQUERY_H_

#include "idmquery.h"
#include "../constraints/idmbaseconstraint.h"


IDM_PLUGIN_NS_BEGIN

class Select : public Query
{
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
	typedef QList<BaseConstraint*> Container;
	Container m_where;
};

IDM_PLUGIN_NS_END

#endif /* IDMSELECTQUERY_H_ */
