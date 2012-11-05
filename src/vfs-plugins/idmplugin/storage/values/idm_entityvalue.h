/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef IDM_ENTITYVALUE_H_
#define IDM_ENTITYVALUE_H_

#include <QtCore/QVariant>
#include <QtCore/QDateTime>
#include <QtCore/QSharedData>
#include <tools/containers/hashedlist.h>
#include "../entities/idm_entity.h"
#include "../queries/idm_querycontext.h"


IDM_PLUGIN_NS_BEGIN

class EntityValue : public QSharedData
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
	typedef Entity::id_type id_type;
	enum { InvalidId = Entity::InvalidId };
	typedef InternalHolder<EntityValue> Holder;

public:
	EntityValue(Entity *entity, id_type id);
	virtual ~EntityValue();

	Entity *entity() const { return m_entity; }

	id_type id() const { return m_id; }
	virtual QVariant value() const = 0;

private:
	Entity *m_entity;
	id_type m_id;
};


class CompositeEntityValue : public EntityValue
{
public:
	typedef QList<EntityValue::Holder> List;

public:
	CompositeEntityValue(Entity *entity, id_type id);

	List values(Entity *property) const { return m_items.value(property).values(); }
	bool contains(const EntityValue::Holder &value) const;
	bool contains(const List &values, EntityValue::Holder &propertyValue) const;

protected:
	typedef ::Tools::Containers::HashedList<id_type, EntityValue::Holder> InternalList;
	typedef QMap<Entity*, InternalList> Map;
	Map m_items;
};


/* Meta-function "EntityValueType" */
template <Database::EntityType EntityType> struct EntityValueType {};
template <> struct EntityValueType<Database::Int>      { typedef int       type; };
template <> struct EntityValueType<Database::String>   { typedef QString   type; };
template <> struct EntityValueType<Database::Date>     { typedef QDate     type; };
template <> struct EntityValueType<Database::Time>     { typedef QTime     type; };
template <> struct EntityValueType<Database::DateTime> { typedef QDateTime type; };
template <> struct EntityValueType<Database::Memo>     { typedef QString   type; };
template <> struct EntityValueType<Database::Rating>   { typedef int       type; };
template <> struct EntityValueType<Database::Path>     { typedef QString   type; };


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
