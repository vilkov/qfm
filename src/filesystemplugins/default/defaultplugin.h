#ifndef DEFAULTPLUGIN_H_
#define DEFAULTPLUGIN_H_

#include "default_ns.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


DEFAULT_PLUGIN_NS_BEGIN

class Plugin : public IPlugin
{
public:
	Plugin();

	virtual void registered();
	virtual Node *node(const IFileContainer *container, const IFileInfo *file, Node *parent) const;

private:
	mutable QString m_error;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTPLUGIN_H_ */
