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
#ifndef M3UITEM_H_
#define M3UITEM_H_

#include <vfs/interfaces/vfs_ifileinfo.h>
#include <tools/models/tree/items/treemodelitem.h>
#include "../m3uplugin_ns.h"


M3U_PLUGIN_NS_BEGIN

class M3uItem : public ::Tools::Models::Tree::Item, public IFileInfo
{
public:
	M3uItem(M3uItem *parent = 0);

	virtual bool isList() const;
	virtual bool isRoot() const = 0;
};

M3U_PLUGIN_NS_END

#endif /* M3UITEM_H_ */
