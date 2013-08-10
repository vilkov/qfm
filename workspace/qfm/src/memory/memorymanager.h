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
#ifndef MEMORYMANAGER_H_
#define MEMORYMANAGER_H_

#include <tools/memory/mm/memorymanagerbase.h>


class MemoryManager : public ::Tools::Memory::ManagerBase<3>
{
public:
	enum Allocators
	{
		Qt,
		Loki,
		Malloc
	};

public:
	MemoryManager();

	static MemoryManager *instance();
	::Tools::Memory::Allocator *allocator(Allocators a) { return allocators()[a]; }
};

#endif /* MEMORYMANAGER_H_ */
