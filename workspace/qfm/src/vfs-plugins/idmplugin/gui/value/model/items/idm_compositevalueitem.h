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
#ifndef IDM_COMPOSITEVALUEITEM_H_
#define IDM_COMPOSITEVALUEITEM_H_

#include "../../../../model/items/idm_item.h"
#include "../../../../storage/values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueItem : public IdmItem
{
public:
	CompositeValueItem(IdmItem *parent = 0);

	virtual bool isPath() const;
	virtual bool isValue() const;
	virtual bool isProperty() const;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEITEM_H_ */
