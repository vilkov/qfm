#ifndef M3UPLUGIN_H_
#define M3UPLUGIN_H_

#include "m3uplugin_ns.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


M3U_PLUGIN_NS_BEGIN

class M3uPlugin : public IPlugin
{
public:
	M3uPlugin();

	virtual Node *node(const IFileInfo *info, Node *parent) const;

private:
	QString m_identity;
};

M3U_PLUGIN_NS_END

#endif /* M3UPLUGIN_H_ */
