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
#ifndef IDM_QUERYRESULTROOTPATHVALUEITEM_H_
#define IDM_QUERYRESULTROOTPATHVALUEITEM_H_

#include <QtCore/QCoreApplication>
#include "idm_queryresultpathitem.h"
#include "../../../storage/values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class QueryResultRootPathValueItem : public QueryResultPathItem
{
	Q_DECLARE_TR_FUNCTIONS(QueryResultPathValueItem)

public:
	QueryResultRootPathValueItem(const IFileContainer *container, const EntityValue::Holder &value, Base *parent);

	/* QueryResultItem */
	virtual bool isRootPathValue();

	/* QueryResultPathItem */
	virtual void open() const;

	void update(SnapshotItem *item);
	const EntityValue::Holder &value() const { return m_value; }

private:
	EntityValue::Holder m_value;
	const IFileContainer *m_container;
	IFileContainer::Holder m_thisContainer;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTROOTPATHVALUEITEM_H_ */
