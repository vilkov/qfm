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
#include "idm_storage_undoremovevalue.h"
#include "../../values/idm_valuereader.h"


IDM_PLUGIN_NS_BEGIN

StorageUndoRemoveValue::StorageUndoRemoveValue(const EntityValue::Holder &entityValue, const EntityValue::Holder &propertyValue) :
	m_entityValue(entityValue),
	m_propertyValue(propertyValue)
{}

void StorageUndoRemoveValue::undo(RootEntity &root)
{
	ValueReader::addValue(m_entityValue, m_propertyValue);
	m_propertyValue = 0;
}

IDM_PLUGIN_NS_END
