#ifndef IDMSELECTQUERY_H_
#define IDMSELECTQUERY_H_

#include "idmquery.h"
#include "../constraints/idmbaseconstraint.h"


IDM_PLUGIN_NS_BEGIN

class Select : public Query
{
public:
	Select(IdmEntity *entity);
	Select(IdmEntity *entity, BaseConstraint *where);

	virtual QByteArray compile() const;

private:
	struct Format
	{
		Format();

		QString select(IdmEntity *entity) const;
		QString join(Database::id_type entity, Database::id_type property) const;
		QString complete(Database::id_type entity, QString &selectedFields, QString &joinedFields, const QString &where) const;

		QString format;
		QString format2;
		QString idField;
		QString valueField;
	};

	void join(const Format &format, QString &selectedFields, QString &joinedFields, IdmEntity *entity, IdmEntity *property) const;

private:
	BaseConstraint::Holder m_where;
};

IDM_PLUGIN_NS_END

#endif /* IDMSELECTQUERY_H_ */
