#include "idmstorageundoaddvalue.h"
#include "../../values/idmvaluereader.h"


IDM_PLUGIN_NS_BEGIN

IdmStorageUndoAddValue::IdmStorageUndoAddValue(IdmEntityValue *value) :
	m_value(value)
{}

void IdmStorageUndoAddValue::undo(IdmEntityRoot &root)
{
	delete m_value;
}

IDM_PLUGIN_NS_END
