#include "idmmodel.h"


IDM_PLUGIN_NS_BEGIN

IdmModel::IdmModel(QObject *parent) :
	Model(m_container, parent),
	m_items(m_container.m_container)
{}

IDM_PLUGIN_NS_END
