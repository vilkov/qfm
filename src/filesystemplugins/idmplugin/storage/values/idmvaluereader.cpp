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

void IdmValueReader::updateValue(IdmEntityValue *value, const QVariant &newValue)
{
	static_cast<IdmEntityValueImp*>(value)->setValue(newValue);
}

void IdmValueReader::removeValue(IdmEntityValue *value, IdmEntityValue *property)
{
	static_cast<IdmEntityCompositeValueImp*>(value)->remove(property);
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
	IdmEntityValue *entity;

	if (m_context.entity()->type() == Database::Composite)
	{
		PScopedPointer<IdmEntityValue> value(createValue(m_context.entity(), m_context.asInt(0)));

		for (IdmEntityValue::id_type id = value->id(), nextId = id; id == nextId; nextId = m_context.asInt(0))
		{
			column = 2;

			for (IdmEntity::size_type i = 0, size = m_context.entity()->size(); i < size; ++i)
				if (entity = property(m_context.entity()->at(i).entity, column))
					addValue(value.data(), entity);

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
		entity = value(m_context.entity(), column);

		if (!m_context.next())
			m_afterLast = true;

		return entity;
	}
}

IdmEntityValue *IdmValueReader::value(IdmEntity *entity, int &column) const
{
	switch (entity->type())
	{
		case Database::Int:
			return createValue(entity, m_context.asInt(column), contextValue<Database::Int>(m_context, column + 1));
		case Database::String:
			return createValue(entity, m_context.asInt(column), contextValue<Database::String>(m_context, column + 1));
		case Database::Date:
			return createValue(entity, m_context.asInt(column), contextValue<Database::Date>(m_context, column + 1));
		case Database::Time:
			return createValue(entity, m_context.asInt(column), contextValue<Database::Time>(m_context, column + 1));
		case Database::DateTime:
			return createValue(entity, m_context.asInt(column), contextValue<Database::DateTime>(m_context, column + 1));
		case Database::Memo:
			return createValue(entity, m_context.asInt(column), contextValue<Database::Memo>(m_context, column + 1));
		case Database::Rating:
			return createValue(entity, m_context.asInt(column), contextValue<Database::Rating>(m_context, column + 1));
		case Database::Path:
			return createValue(entity, m_context.asInt(column), contextValue<Database::Path>(m_context, column + 1));
	}
}

IdmEntityValue *IdmValueReader::property(IdmEntity *entity, int &column) const
{
	if (m_context.columnType(column) == QueryContext::Null)
		column += 2;
	else
	{
		IdmEntityValue *tmp;

		if (entity->type() == Database::Composite)
		{
			PScopedPointer<IdmEntityValue> value(createValue(entity, m_context.asInt(column)));

			column += 2;
			for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
				if (tmp = property(entity->at(i).entity, column))
					addValue(value.data(), tmp);

			return value.take();
		}
		else
		{
			tmp = value(entity, column);
			column += 2;

			return tmp;
		}
	}

	return 0;
}

IDM_PLUGIN_NS_END
