#ifndef IDMPLUGIN_H_
#define IDMPLUGIN_H_

#include "actions/idmcreatedbaction.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


IDM_PLUGIN_NS_BEGIN

class Plugin : public IPlugin
{
public:
	Plugin();

	virtual void registered();
	virtual Node *node(const IFileInfo *info, Node *parent) const;

private:
	CreateDbAction m_createDbAction;
};

IDM_PLUGIN_NS_END

#endif /* IDMPLUGIN_H_ */
