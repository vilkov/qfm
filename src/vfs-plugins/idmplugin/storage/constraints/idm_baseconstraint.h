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
#ifndef IDM_BASECONSTRAINT_H_
#define IDM_BASECONSTRAINT_H_

#include <QtCore/QSharedData>
#include <tools/memory/memory_holder.h>
#include "../entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class BaseConstraint : public QSharedData
{
public:
    typedef ::Tools::Memory::Holder<BaseConstraint> Holder;

public:
	BaseConstraint(BaseConstraint *parent = 0);
	virtual ~BaseConstraint();

	BaseConstraint *parent() const { return m_parent; }

	virtual bool isGroup() const = 0;
	virtual QString toString() const = 0;

private:
	BaseConstraint *m_parent;
};

IDM_PLUGIN_NS_END

#endif /* IDM_BASECONSTRAINT_H_ */
