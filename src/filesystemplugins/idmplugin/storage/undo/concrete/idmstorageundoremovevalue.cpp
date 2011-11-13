#include "idmstorageundoremovevalue.h"
#include "../../values/idmvaluereader.h"


IDM_PLUGIN_NS_BEGIN

IdmStorageUndoRemoveValue::IdmStorageUndoRemoveValue(IdmCompositeEntityValue *entityValue, IdmEntityValue *propertyValue) :
	m_entityValue(entityValue),
	m_propertyValue(propertyValue)
{}

IdmStorageUndoRemoveValue::~IdmStorageUndoRemoveValue()
{
	delete m_propertyValue;
}

void IdmStorageUndoRemoveValue::undo(IdmEntityRoot &root)
{
	IdmValueReader::addValue(m_entityValue, m_propertyValue);
	m_propertyValue = 0;
}

IDM_PLUGIN_NS_END
