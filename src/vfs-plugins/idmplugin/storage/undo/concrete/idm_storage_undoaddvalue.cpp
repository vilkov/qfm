#include "idm_storage_undoaddvalue.h"
#include "../../values/idm_valuereader.h"


IDM_PLUGIN_NS_BEGIN

StorageUndoAddValue::StorageUndoAddValue(const EntityValue::Holder &entityValue, const EntityValue::Holder &propertyValue) :
	m_entityValue(entityValue)
{
	m_propertyValues.push_back(propertyValue);
}

StorageUndoAddValue::StorageUndoAddValue(const EntityValue::Holder &entityValue, const CompositeEntityValue::List &propertyValues) :
	m_entityValue(entityValue),
	m_propertyValues(propertyValues)
{}

void StorageUndoAddValue::undo(RootEntity &root)
{
	ValueReader::removeValue(m_entityValue, m_propertyValues);
}

IDM_PLUGIN_NS_END
