#ifndef IDMENTITYVALUE_H_
#define IDMENTITYVALUE_H_

#include <QtCore/QVariant>
#include <QtCore/QDateTime>
#include "../entities/idmentity.h"
#include "../queries/idmquerycontext.h"
#include "../../../../tools/containers/hashedlist.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntityValue
{
public:
	typedef IdmEntity::id_type id_type;
	enum { InvalidId = IdmEntity::InvalidId };

public:
	IdmEntityValue(IdmEntity *entity, id_type id);
	virtual ~IdmEntityValue();

	IdmEntity *entity() const { return m_entity; }

	id_type id() const { return m_id; }
	virtual QVariant value() const = 0;

private:
	IdmEntity *m_entity;
	id_type m_id;
};


class IdmCompositeEntityValue : public IdmEntityValue
{
public:
	typedef QList<IdmEntityValue*> List;

public:
	IdmCompositeEntityValue(IdmEntity *entity, id_type id);
	virtual ~IdmCompositeEntityValue();

	List values(IdmEntity *property) const { return m_items.value(property).values(); }
	bool contains(IdmEntityValue *value) const;
	bool contains(const List &values, IdmEntityValue *&propertyValue) const;

protected:
	typedef ::Tools::Containers::HashedList<id_type, IdmEntityValue*> InternalList;
	typedef QMap<IdmEntity*, InternalList> Map;
	Map m_items;
};


/* Meta-function "EntityValueType" */
template <Database::EntityType EntityType> struct EntityValueType {};
template <> struct EntityValueType<Database::Int>       { typedef int       type; };
template <> struct EntityValueType<Database::String>    { typedef QString   type; };
template <> struct EntityValueType<Database::Date>      { typedef QDate     type; };
template <> struct EntityValueType<Database::Time>      { typedef QTime     type; };
template <> struct EntityValueType<Database::DateTime>  { typedef QDateTime type; };
template <> struct EntityValueType<Database::Memo>      { typedef QString   type; };
template <> struct EntityValueType<Database::Rating>    { typedef int       type; };
template <> struct EntityValueType<Database::Path>      { typedef QString   type; };


/* Function "contextValue" */
template <Database::EntityType EntityType>
inline QVariant contextValue(const QueryContext &context, int column);
template <> inline QVariant contextValue<Database::Int>(const QueryContext &context, int column)      { return context.asInt(column); }
template <> inline QVariant contextValue<Database::String>(const QueryContext &context, int column)   { return context.asText(column); }
template <> inline QVariant contextValue<Database::Date>(const QueryContext &context, int column)     { return QDate(); }
template <> inline QVariant contextValue<Database::Time>(const QueryContext &context, int column)     { return QTime(); }
template <> inline QVariant contextValue<Database::DateTime>(const QueryContext &context, int column) { return QDateTime(); }
template <> inline QVariant contextValue<Database::Memo>(const QueryContext &context, int column)     { return context.asText(column); }
template <> inline QVariant contextValue<Database::Rating>(const QueryContext &context, int column)   { return context.asInt(column); }
template <> inline QVariant contextValue<Database::Path>(const QueryContext &context, int column)     { return context.asText(column); }

IDM_PLUGIN_NS_END

#endif /* IDMENTITYVALUE_H_ */
