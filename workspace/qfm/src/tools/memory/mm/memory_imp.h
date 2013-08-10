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
#ifndef MEMORY_IMP_H_
#define MEMORY_IMP_H_

#include <QMap>
#include <QMutex>
#include <stdlib.h>
#include "memory.h"


namespace Memory
{
	static QMutex mutex;
	static size_t leaks = 0;
	static QMap<size_t, size_t> statistics;
}

void *operator new(size_t size) throw (std::bad_alloc)
{
	QMutex::Locker locker(&Memory::mutex);
	++Memory::statistics[size];
	Memory::leaks += size;
    return malloc(size);
}

void *operator new[](size_t size) throw (std::bad_alloc)
{
	QMutex::Locker locker(&Memory::mutex);
	++Memory::statistics[size];
	Memory::leaks += size;
    return malloc(size);
}

void operator delete(void *ptr) throw ()
{
	QMutex::Locker locker(&Memory::mutex);
    free(ptr);
}

void operator delete(void *ptr, size_t size) throw ()
{
	QMutex::Locker locker(&Memory::mutex);
	Memory::leaks -= size;
    free(ptr);
}

void operator delete[](void *ptr) throw ()
{
	QMutex::Locker locker(&Memory::mutex);
    free(ptr);
}

void operator delete[](void *ptr, size_t size) throw ()
{
	QMutex::Locker locker(&Memory::mutex);
	Memory::leaks -= size;
    free(ptr);
}

#endif /* MEMORY_IMP_H_ */
