#include "idmvaluereader.h"
#include "idmvaluereader_p.h"
#include "../../../../tools/pointers/pscopedpointer.h"


IDM_PLUGIN_NS_BEGIN

IdmValueReader::IdmValueReader(const IdmContainer &container, const Select &query) :
	m_context(container.prepare(query, m_lastError)),
	m_afterLast(!isValid()),
	m_beforeFirst(isValid())
{}

IdmEntityValue *IdmValueReader::next() const
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
			return 0;
		}
	else
		if (m_afterLast)
			return 0;
		else
			return doNext();
}

void IdmValueReader::addValue(IdmEntityValue *value, IdmEntityValue *property)
{
	static_cast<IdmEntityCompositeValueImp*>(value)->add(property);
}

void IdmValueReader::addValue(IdmEntityValue *value, const IdmCompositeEntityValue::List &values)
{
	static_cast<IdmEntityCompositeValueImp*>(value)->add(values);
}

void IdmValueReader::takeValue(IdmEntityValue *value, IdmEntityValue *property)
{
	static_cast<IdmEntityCompositeValueImp*>(value)->take(property);
}

void IdmValueReader::updateValue(IdmEntityValue *value, const QVariant &newValue)
{
	static_cast<IdmEntityValueImp*>(value)->setValue(newValue);
}

void IdmValueReader::removeValue(IdmEntityValue *value, IdmEntityValue *property)
{
	static_cast<IdmEntityCompositeValueImp*>(value)->remove(property);
}

void IdmValueReader::removeValue(IdmEntityValue *value, const IdmCompositeEntityValue::List &values)
{
	static_cast<IdmEntityCompositeValueImp*>(value)->remove(values);
}

IdmCompositeEntityValue *IdmValueReader::createValue(IdmEntity *entity, IdmEntityValue::id_type id)
{
	return new IdmEntityCompositeValueImp(entity, id);
}

IdmEntityValue *IdmValueReader::createValue(IdmEntity *entity, IdmEntityValue::id_type id, const QVariant &value)
{
	return new IdmEntityValueImp(entity, id, value);
}

IdmEntityValue *IdmValueReader::doNext() const
{
	int column = 0;

	if (m_context.entity()->type() == Database::Composite)
	{
		PScopedPointer<IdmEntityValue> value(createValue(m_context.entity(), m_context.asInt(0)));

		for (IdmEntityValue::id_type id = value->id(), nextId = id; id == nextId; nextId = m_context.asInt(0))
		{
			column = 1;

			for (IdmEntity::size_type i = 0, size = m_context.entity()->size(); i < size; ++i)
				property(value.data(), m_context.entity()->at(i).entity, column);

			if (!m_context.next())
			{
				m_afterLast = true;
				break;
			}
		}

		return value.take();
	}
	else
	{
		IdmEntityValue *property = value(m_context.entity(), m_context.asInt(0), column);

		if (!m_context.next())
			m_afterLast = true;

		return property;
	}
}

IdmEntityValue *IdmValueReader::value(IdmEntity *entity, IdmEntity::id_type id, int column) const
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
			return 0;
	}
}

void IdmValueReader::property(IdmEntityValue *value, IdmEntity *property, int &column) const
{
	if (m_context.columnType(column) == QueryContext::Null)
		skip(property, column);
	else
	{
		IdmEntity::id_type id = m_context.asInt(column);

		if (IdmEntityValue *lastValue = static_cast<IdmEntityCompositeValueImp*>(value)->value(property, id))
		{
			if (property->type() == Database::Composite)
			{
				column += 1;

				for (IdmEntity::size_type i = 0, size = property->size(); i < size; ++i)
					IdmValueReader::property(lastValue, property->at(i).entity, column);
			}
			else
				column += 2;

			return;
		}

		if (property->type() == Database::Composite)
		{
			PScopedPointer<IdmEntityValue> localValue(createValue(property, id));
			column += 1;

			for (IdmEntity::size_type i = 0, size = property->size(); i < size; ++i)
				IdmValueReader::property(localValue.data(), property->at(i).entity, column);

			addValue(value, localValue.take());
		}
		else
		{
			addValue(value, IdmValueReader::value(property, id, column));
			column += 2;
		}
	}
}

void IdmValueReader::skip(IdmEntity *property, int &column) const
{
	if (property->type() == Database::Composite)
	{
		column += 1;

		for (IdmEntity::size_type i = 0, size = property->size(); i < size; ++i)
			skip(property->at(i).entity, column);
	}
	else
		column += 2;
}

IDM_PLUGIN_NS_END
