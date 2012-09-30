#include "idm_storage_undoupdatevalue.h"
#include "../../values/idm_valuereader.h"


IDM_PLUGIN_NS_BEGIN

StorageUndoUpdateValue::StorageUndoUpdateValue(const EntityValue::Holder &value) :
	m_value(value),
	m_oldValue(m_value->value())
{}

void StorageUndoUpdateValue::undo(RootEntity &root)
{
	ValueReader::updateValue(m_value, m_oldValue);
}

IDM_PLUGIN_NS_END
