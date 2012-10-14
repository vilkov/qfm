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

		do
		{
			next = m_current->m_next;
			delete m_current;
			m_current = next;
		}
		while (m_current);
	}
}

void Navigation::init(Entry *entry)
{
	Q_ASSERT(entry);

	m_current = entry;
	m_current->m_head = m_current;
	m_current->m_next = NULL;
	m_current->m_prev = NULL;
}

void Navigation::save(Entry *entry)
{
	Q_ASSERT(m_current);

	if (m_current->m_next)
		if (m_current->m_next->isEqual(entry))
		{
			m_current = m_current->m_next;
			delete entry;
			return;
		}
		else
			if (m_current->m_prev && m_current->m_prev->isEqual(entry))
			{
				m_current = m_current->m_prev;
				delete entry;
				return;
			}
			else
			{
				Entry *next;
				Entry *entry = m_current->m_next;

				do
				{
					next = entry->m_next;
					delete entry;
					entry = next;
				}
				while (entry);
			}
	else
		if (m_current->m_prev && m_current->m_prev->isEqual(entry))
		{
			m_current = m_current->m_prev;
			delete entry;
			return;
		}

	m_current->m_next = entry;
	entry->m_head = m_current->m_head;
	entry->m_prev = m_current;
	entry->m_next = NULL;

	m_current = entry;
}

Entry *Navigation::backward()
{
	Q_ASSERT(m_current);

	if (m_current->m_prev)
	{
		m_current = m_current->m_prev;
		return m_current;
	}

	return NULL;
}

Entry *Navigation::forward()
{
	Q_ASSERT(m_current);

	if (m_current->m_next)
	{
		m_current = m_current->m_next;
		return m_current;
	}

	return NULL;
}

HISTORY_NS_END
