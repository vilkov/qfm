#ifndef IDM_ENTITYVALUE_H_
#define IDM_ENTITYVALUE_H_

#include <QtCore/QVariant>
#include <QtCore/QDateTime>
#include <QtCore/QSharedData>
#include "../entities/idm_entity.h"
#include "../queries/idm_querycontext.h"
#include "../../../../tools/containers/hashedlist.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntityValue : public QSharedData
{
private:
	template <typename T>
	class InternalHolder
	{
	public:
		typedef InternalHolder<T> class_type;

	public:
	    inline InternalHolder() :
	    	m_data()
	    {}
	    explicit InternalHolder(T *p) :
	    	m_data(p)
	    {}

	    inline T &operator*() const { return m_data.operator *(); }
	    inline T *operator->() const { return m_data.operator->(); }
	    inline T *operator->() { return m_data.operator->(); }
	    inline operator bool() const { return m_data.operator bool(); }

	    inline class_type &operator=(T *p) { m_data.operator =(p); return *this; }
	    inline void reset() { m_data.reset(); }

	    inline T *data() const { return m_data.data(); }

	    template <typename R> inline
	    R *as() const { return static_cast<R *>(m_data.data()); }

	private:
	    QExplicitlySharedDataPointer<T> m_data;
	};

public:
	typedef IdmEntity::id_type id_type;
	enum { InvalidId = IdmEntity::InvalidId };
	typedef InternalHolder<IdmEntityValue> Holder;

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
	typedef QList<IdmEntityValue::Holder> List;

public:
	IdmCompositeEntityValue(IdmEntity *entity, id_type id);

	List values(IdmEntity *property) const { return m_items.value(property).values(); }
	bool contains(const IdmEntityValue::Holder &value) const;
	bool contains(const List &values, IdmEntityValue::Holder &propertyValue) const;

protected:
	typedef ::Tools::Containers::HashedList<id_type, IdmEntityValue::Holder> InternalList;
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

#endif /* IDM_ENTITYVALUE_H_ */
