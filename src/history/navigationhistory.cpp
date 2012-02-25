#include "navigationhistory.h"
#include <stddef.h>
#include <qglobal.h>


HISTORY_NS_BEGIN

Navigation::Navigation() :
	m_current(NULL)
{}

Navigation::~Navigation()
{
	if (m_current)
	{
		Entry *next;
		m_current = m_current->m_head;

		while (m_current)
		{
			next = m_current->m_next;
			delete m_current;
			m_current = next;
		}
	}
}

void Navigation::insert(Entry *entry)
{
	if (m_current)
	{
		removeTail(m_current);

		m_current->m_next = entry;
		entry->m_head = m_current->m_head;
		entry->m_prev = m_current;
		entry->m_next = NULL;
	}
	else
	{
		entry->m_head = entry;
		entry->m_prev = NULL;
		entry->m_next = NULL;
	}

	m_current = entry;
}

Entry *Navigation::backward()
{
	Q_ASSERT(m_current);

	if (m_current->m_prev)
		return m_current = m_current->m_prev;

	return NULL;
}

Entry *Navigation::forvard()
{
	Q_ASSERT(m_current);

	if (m_current->m_next)
		return m_current = m_current->m_next;

	return NULL;
}

void Navigation::removeTail(Entry *entry)
{
	Q_ASSERT(entry);

	if (entry = entry->m_next)
	{
		Entry *next;

		do
		{
			next = entry->m_next;
			delete entry;
			entry = next;
		}
		while (entry);
	}
}

HISTORY_NS_END
