#include "idmstorageundoupdatevalue.h"
#include "../../values/idmvaluereader.h"


IDM_PLUGIN_NS_BEGIN

IdmStorageUndoUpdateValue::IdmStorageUndoUpdateValue(IdmEntityValue *value) :
	m_value(value),
	m_oldValue(m_value->value())
{}

void IdmStorageUndoUpdateValue::undo(IdmEntityRoot &root)
{
	IdmValueReader::updateValue(m_value, m_oldValue);
}

IDM_PLUGIN_NS_END
