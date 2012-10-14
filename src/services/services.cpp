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
#include "services.h"


static Services *selfInstance = 0;

Services::Services()
{
	selfInstance = this;
}

//void Services::view(const ::VFS::Info &file)
//{
//
//}
//
//void Services::edit(const ::VFS::Info &file)
//{
//
//}
//
//void Services::open(const ::VFS::Info &file)
//{
//
//}

Services *Services::instance()
{
	Q_ASSERT(selfInstance != 0);
	return selfInstance;
}

void Services::registerService(View *service)
{
	Q_ASSERT(selfInstance != 0);
}

void Services::registerService(Edit *service)
{
	Q_ASSERT(selfInstance != 0);
}

void Services::registerService(Open *service)
{
	Q_ASSERT(selfInstance != 0);
}
