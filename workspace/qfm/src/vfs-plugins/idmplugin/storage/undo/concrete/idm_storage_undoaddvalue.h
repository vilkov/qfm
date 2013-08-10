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
#ifndef IDM_STORAGE_UNDOADDVALUE_H_
#define IDM_STORAGE_UNDOADDVALUE_H_

#include "../idm_storage_undocommand.h"
#include "../../values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class StorageUndoAddValue : public StorageUndoCommand
{
public:
	StorageUndoAddValue(const EntityValue::Holder &entityValue, const EntityValue::Holder &propertyValue);
	StorageUndoAddValue(const EntityValue::Holder &entityValue, const CompositeEntityValue::List &propertyValues);

	virtual void undo(RootEntity &root);

private:
	EntityValue::Holder m_entityValue;
	CompositeEntityValue::List m_propertyValues;
};

IDM_PLUGIN_NS_END

#endif /* IDM_STORAGE_UNDOADDVALUE_H_ */
