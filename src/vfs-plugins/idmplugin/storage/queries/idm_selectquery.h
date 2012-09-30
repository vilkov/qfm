#ifndef IDM_SELECTQUERY_H_
#define IDM_SELECTQUERY_H_

#include "idm_query.h"
#include "../constraints/idm_baseconstraint.h"


IDM_PLUGIN_NS_BEGIN

class Select : public Query
{
public:
	Select(Entity *entity);
	Select(Entity *entity, BaseConstraint *where);

	virtual QByteArray compile() const;

private:
	struct Format
	{
		Format();

		QString select(Entity *entity) const;
		QString join(Database::id_type entity, Database::id_type property) const;
		QString complete(Database::id_type entity, QString &selectedFields, QString &joinedFields, const QString &where) const;

		QString format;
		QString format2;
		QString idField;
		QString valueField;
	};

	void join(const Format &format, QString &selectedFields, QString &joinedFields, Entity *entity, Entity *property) const;

private:
	BaseConstraint::Holder m_where;
};

IDM_PLUGIN_NS_END

#endif /* IDM_SELECTQUERY_H_ */
