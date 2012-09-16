#include "idm_storage_undoremovevalue.h"
#include "../../values/idm_valuereader.h"


IDM_PLUGIN_NS_BEGIN

IdmStorageUndoRemoveValue::IdmStorageUndoRemoveValue(const IdmEntityValue::Holder &entityValue, const IdmEntityValue::Holder &propertyValue) :
	m_entityValue(entityValue),
	m_propertyValue(propertyValue)
{}

void IdmStorageUndoRemoveValue::undo(IdmEntityRoot &root)
{
	IdmValueReader::addValue(m_entityValue, m_propertyValue);
	m_propertyValue = 0;
}

IDM_PLUGIN_NS_END
