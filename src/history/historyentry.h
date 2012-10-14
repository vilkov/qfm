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
#ifndef HISTORYENTRY_H_
#define HISTORYENTRY_H_

#include "history_ns.h"


HISTORY_NS_BEGIN

class Entry
{
public:
	virtual ~Entry();

	virtual bool isEqual(const Entry *entry) const = 0;

private:
	friend class Navigation;
	Entry *m_head;
	Entry *m_prev;
	Entry *m_next;
};

HISTORY_NS_END

#endif /* HISTORYENTRY_H_ */
