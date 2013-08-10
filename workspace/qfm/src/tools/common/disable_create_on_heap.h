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
#ifndef DISABLE_CREATE_ON_HEAP_H_
#define DISABLE_CREATE_ON_HEAP_H_

#include <new>

#define DISABLE_CREATE_ON_HEAP                                                     \
	void* operator new(std::size_t) throw (std::bad_alloc) { return 0; }           \
	void* operator new[](std::size_t) throw (std::bad_alloc) { return 0; }         \
	void operator delete(void*) throw() {}                                         \
	void operator delete[](void*) throw() {}                                       \
	void* operator new(std::size_t, const std::nothrow_t&) throw() { return 0; }   \
	void* operator new[](std::size_t, const std::nothrow_t&) throw() { return 0; } \
	void operator delete(void*, const std::nothrow_t&) throw() {}                  \
	void operator delete[](void*, const std::nothrow_t&) throw() {}                \
                                                                                   \
	inline void* operator new(std::size_t, void* __p) throw() { return __p; }      \
	inline void* operator new[](std::size_t, void* __p) throw() { return __p; }    \
                                                                                   \
	inline void  operator delete  (void*, void*) throw() {}                        \
	inline void  operator delete[](void*, void*) throw() {}                        \


#endif /* DISABLE_CREATE_ON_HEAP_H_ */
