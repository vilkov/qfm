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
#include "memorymanager.h"
#include "allocators/qt_memory_allocator.h"
#include "allocators/loki_memory_allocator.h"
#include "allocators/malloc_memory_allocator.h"


static MemoryManager *instance = 0;
static QtAllocator qtAllocator;
static LokiAllocator lokiAllocator(4096, 64, 4);
static MallocAllocator mallocAllocator;


MemoryManager::MemoryManager()
{
	::instance = this;
	allocators()[Qt] = &qtAllocator;
	allocators()[Loki] = &lokiAllocator;
	allocators()[Malloc] = &mallocAllocator;
}

MemoryManager *MemoryManager::instance()
{
	return ::instance;
}
