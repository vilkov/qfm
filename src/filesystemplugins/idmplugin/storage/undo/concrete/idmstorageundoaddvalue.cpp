#include "idmstorageundoaddvalue.h"
#include "../../values/idmvaluereader.h"


IDM_PLUGIN_NS_BEGIN

IdmStorageUndoAddValue::IdmStorageUndoAddValue(const IdmEntityValue::Holder &entityValue, const IdmEntityValue::Holder &propertyValue) :
	m_entityValue(entityValue)
{
	m_propertyValues.push_back(propertyValue);
}

IdmStorageUndoAddValue::IdmStorageUndoAddValue(const IdmEntityValue::Holder &entityValue, const IdmCompositeEntityValue::List &propertyValues) :
	m_entityValue(entityValue),
	m_propertyValues(propertyValues)
{}

void IdmStorageUndoAddValue::undo(IdmEntityRoot &root)
{
	IdmValueReader::removeValue(m_entityValue, m_propertyValues);
}

IDM_PLUGIN_NS_END
