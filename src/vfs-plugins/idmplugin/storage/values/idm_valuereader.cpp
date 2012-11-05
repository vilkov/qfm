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
#include "idm_valuereader.h"
#include "idm_valuereader_p.h"

#include <tools/pointers/pscopedpointer.h>


IDM_PLUGIN_NS_BEGIN

ValueReader::ValueReader(const IdmContainer &container, const Select &query) :
	m_context(container.prepare(query, m_lastError)),
	m_afterLast(!isValid()),
	m_beforeFirst(isValid())
{}

EntityValue::Holder ValueReader::next() const
{
	if (m_beforeFirst)
		if (m_context.next())
		{
			m_beforeFirst = false;
			return doNext();
		}
		else
		{
			m_beforeFirst = false;
			m_afterLast = true;
			return EntityValue::Holder();
		}
	else
		if (m_afterLast)
			return EntityValue::Holder();
		else
			return doNext();
}

void ValueReader::addValue(const EntityValue::Holder &value, const EntityValue::Holder &property)
{
	static_cast<CompositeEntityValueImp*>(value.data())->add(property);
}

void ValueReader::addValue(const EntityValue::Holder &value, const CompositeEntityValue::List &values)
{
	static_cast<CompositeEntityValueImp*>(value.data())->add(values);
}

void ValueReader::takeValue(const EntityValue::Holder &value, const EntityValue::Holder &property)
{
	static_cast<CompositeEntityValueImp*>(value.data())->take(property);
}

void ValueReader::updateValue(const EntityValue::Holder &value, const QVariant &newValue)
{
	static_cast<EntityValueImp*>(value.data())->setValue(newValue);
}

void ValueReader::removeValue(const EntityValue::Holder &value, const EntityValue::Holder &property)
{
	static_cast<CompositeEntityValueImp*>(value.data())->remove(property);
}

void ValueReader::removeValue(const EntityValue::Holder &value, const CompositeEntityValue::List &values)
{
	static_cast<CompositeEntityValueImp*>(value.data())->remove(values);
}

EntityValue::Holder ValueReader::createValue(Entity *entity, EntityValue::id_type id)
{
	return EntityValue::Holder(new CompositeEntityValueImp(entity, id));
}

EntityValue::Holder ValueReader::createValue(Entity *entity, EntityValue::id_type id, const QVariant &value)
{
	return EntityValue::Holder(new EntityValueImp(entity, id, value));
}

EntityValue::Holder ValueReader::doNext() const
{
	int column = 0;

	if (m_context.entity()->type() == Database::Composite)
	{
		EntityValue::Holder value(createValue(m_context.entity(), m_context.asInt(0)));

		for (EntityValue::id_type id = value->id(), nextId = id; id == nextId; nextId = m_context.asInt(0))
		{
			column = 1;

			for (Entity::size_type i = 0, size = m_context.entity()->size(); i < size; ++i)
				property(value, m_context.entity()->at(i).entity, column);

			if (!m_context.next())
			{
				m_afterLast = true;
				break;
			}
		}

		return value;
	}
	else
	{
		EntityValue::Holder property = value(m_context.entity(), m_context.asInt(0), column);

		if (!m_context.next())
			m_afterLast = true;

		return property;
	}
}

EntityValue::Holder ValueReader::value(Entity *entity, Entity::id_type id, int column) const
{
	switch (entity->type())
	{
		case Database::Int:
			return createValue(entity, id, contextValue<Database::Int>(m_context, column + 1));
		case Database::String:
			return createValue(entity, id, contextValue<Database::String>(m_context, column + 1));
		case Database::Date:
			return createValue(entity, id, contextValue<Database::Date>(m_context, column + 1));
		case Database::Time:
			return createValue(entity, id, contextValue<Database::Time>(m_context, column + 1));
		case Database::DateTime:
			return createValue(entity, id, contextValue<Database::DateTime>(m_context, column + 1));
		case Database::Memo:
			return createValue(entity, id, contextValue<Database::Memo>(m_context, column + 1));
		case Database::Rating:
			return createValue(entity, id, contextValue<Database::Rating>(m_context, column + 1));
		case Database::Path:
			return createValue(entity, id, contextValue<Database::Path>(m_context, column + 1));
		default:
			return EntityValue::Holder();
	}
}

void ValueReader::property(const EntityValue::Holder &value, Entity *property, int &column) const
{
	if (m_context.columnType(column) == QueryContext::Null)
		skip(property, column);
	else
	{
		Entity::id_type id = m_context.asInt(column);
		EntityValue::Holder lastValue;

		if (lastValue = static_cast<CompositeEntityValueImp*>(value.data())->value(property, id))
		{
			if (property->type() == Database::Composite)
			{
				column += 1;

				for (Entity::size_type i = 0, size = property->size(); i < size; ++i)
					ValueReader::property(lastValue, property->at(i).entity, column);
			}
			else
				column += 2;

			return;
		}

		if (property->type() == Database::Composite)
		{
			EntityValue::Holder localValue(createValue(property, id));
			column += 1;

			for (Entity::size_type i = 0, size = property->size(); i < size; ++i)
				ValueReader::property(localValue, property->at(i).entity, column);

			addValue(value, localValue);
		}
		else
		{
			addValue(value, ValueReader::value(property, id, column));
			column += 2;
		}
	}
}

void ValueReader::skip(Entity *property, int &column) const
{
	if (property->type() == Database::Composite)
	{
		column += 1;

		for (Entity::size_type i = 0, size = property->size(); i < size; ++i)
			skip(property->at(i).entity, column);
	}
	else
		column += 2;
}

IDM_PLUGIN_NS_END
