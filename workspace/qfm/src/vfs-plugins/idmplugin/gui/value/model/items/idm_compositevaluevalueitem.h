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
#ifndef IDM_COMPOSITEVALUEVALUEITEM_H_
#define IDM_COMPOSITEVALUEVALUEITEM_H_

#include "idm_compositevalueitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValueValueItem : public CompositeValueItem
{
public:
	CompositeValueValueItem(const EntityValue::Holder &value, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* CompositeValueItem */
	virtual bool isValue() const;

	const EntityValue::Holder &value() const { return m_value; }
	EntityValue::Holder take() { EntityValue::Holder res(m_value); m_value.reset(); return res; }

protected:
	EntityValue::Holder m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEVALUEITEM_H_ */
