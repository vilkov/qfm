#ifndef IDMFILECONTROL_H_
#define IDMFILECONTROL_H_

#include "../idmplugin_ns.h"
#include "../storage/entities/idmentity.h"


IDM_PLUGIN_NS_BEGIN

class IdmFileControl
{
public:
	IdmFileControl(IdmEntity *entity);

private:
	IdmEntity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDMFILECONTROL_H_ */
