#ifndef IDMCOPYCONTROL_H_
#define IDMCOPYCONTROL_H_

#include <QtCore/QCoreApplication>
#include "../idmplugin_ns.h"
#include "../containeres/idmcontainer.h"
#include "../storage/entities/idmentity.h"
#include "../../../filesystem/info/filesystemcopyinfo.h"


IDM_PLUGIN_NS_BEGIN

class IdmCopyControl : public CopyInfo
{
	Q_DECLARE_TR_FUNCTIONS(IdmCopyControl)

public:
	IdmCopyControl(const IdmContainer &container, IdmEntity *entity, const Info &info);

	/* ICopyControl */
	virtual bool start(const FileSystemList *files, bool move);
	virtual void done(bool error);
	virtual void canceled();

private:
	IdmContainer m_container;
	IdmEntity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDMCOPYCONTROL_H_ */
