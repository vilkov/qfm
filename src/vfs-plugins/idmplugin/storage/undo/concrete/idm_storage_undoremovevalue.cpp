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
