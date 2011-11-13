#include "idmstorageundoaddvalue.h"
#include "../../values/idmvaluereader.h"


IDM_PLUGIN_NS_BEGIN

IdmStorageUndoAddValue::IdmStorageUndoAddValue(IdmCompositeEntityValue *entityValue, IdmEntityValue *propertyValue) :
	m_entityValue(entityValue),
	m_propertyValue(propertyValue)
{}

void IdmStorageUndoAddValue::undo(IdmEntityRoot &root)
{
	IdmValueReader::removeValue(m_entityValue, m_propertyValue);
}

IDM_PLUGIN_NS_END
