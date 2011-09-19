#include "idmvaluereader.h"
#include "idmvaluereader_p.h"
#include "../../../../tools/pointers/pscopedpointer.h"


IDM_PLUGIN_NS_BEGIN

IdmValueReader::IdmValueReader(const IdmContainer &container, const Select &query) :
	m_context(container.prepare(query, m_lastError)),
	m_afterLast(false),
	m_beforeFirst(true)
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
			return 0;
	else
		if (m_afterLast)
			return 0;
		else
			return doNext();
}

IdmEntityValue *IdmValueReader::doNext() const
{
	int column = 0;

	if (m_context.entity()->type() == Database::Composite)
	{
		IdmEntityValue *entity;
		PScopedPointer<IdmEntityValue> value(new IdmEntityCompositeValueImp(m_context.entity(), m_context.asInt(0)));

		for (IdmEntityValue::id_type id = value->id(), nextId = id; id == nextId; nextId = m_context.asInt(0))
		{
			column = 2;

			for (IdmEntity::size_type i = 0, size = m_context.entity()->size(); i < size; ++i)
				if (entity = property(m_context.entity()->at(i).entity, column))
					static_cast<IdmEntityCompositeValueImp*>(value.data())->add(entity);

			if (!m_context.next())
			{
				m_afterLast = true;
				break;
			}
		}

		return value.take();
	}
	else
		return value(m_context.entity(), column);

	return 0;
}

IdmEntityValue *IdmValueReader::value(IdmEntity *entity, int &column) const
{
	switch (entity->type())
	{
		case Database::Int:
			return new IdmEntityValueImp(entity, m_context.asInt(column), contextValue<Database::Int>(m_context, column + 1));
		case Database::String:
			return new IdmEntityValueImp(entity, m_context.asInt(column), contextValue<Database::String>(m_context, column + 1));
		case Database::Date:
			return new IdmEntityValueImp(entity, m_context.asInt(column), contextValue<Database::Date>(m_context, column + 1));
		case Database::Time:
			return new IdmEntityValueImp(entity, m_context.asInt(column), contextValue<Database::Time>(m_context, column + 1));
		case Database::DateTime:
			return new IdmEntityValueImp(entity, m_context.asInt(column), contextValue<Database::DateTime>(m_context, column + 1));
		case Database::Memo:
			return new IdmEntityValueImp(entity, m_context.asInt(column), contextValue<Database::Memo>(m_context, column + 1));
		case Database::Rating:
			return new IdmEntityValueImp(entity, m_context.asInt(column), contextValue<Database::Rating>(m_context, column + 1));
		case Database::Path:
			return new IdmEntityValueImp(entity, m_context.asInt(column), contextValue<Database::Path>(m_context, column + 1));
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
			PScopedPointer<IdmEntityValue> value(new IdmEntityCompositeValueImp(entity, m_context.asInt(column)));

			column += 2;
			for (IdmEntity::size_type i = 0, size = entity->size(); i < size; ++i)
				if (tmp = property(entity->at(i).entity, column))
					static_cast<IdmEntityCompositeValueImp*>(value.data())->add(tmp);

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
