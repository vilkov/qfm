#include "idmcreatedbaction.h"


IDM_PLUGIN_NS_BEGIN

CreateDbAction::CreateDbAction() :
	m_action(tr("Create database..."), 0)
{}

const QAction *CreateDbAction::action() const
{
	return &m_action;
}

void CreateDbAction::process(const FilesList &files) const
{

}

IDM_PLUGIN_NS_END
