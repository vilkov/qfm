#ifndef IDMCOPYCONTROL_H_
#define IDMCOPYCONTROL_H_

#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include "../../../containeres/idmcontainer.h"
#include "../../../storage/entities/idmentity.h"
#include "../../../../../filesystem/info/filesystemcopycontrol.h"


IDM_PLUGIN_NS_BEGIN

class IdmCopyControl : public CopyControl
{
	Q_DECLARE_TR_FUNCTIONS(IdmCopyControl)

public:
	IdmCopyControl(const IdmContainer &container, IdmEntity *entity, const Info &info);

	/* ICopyControl */
	virtual bool start(const Snapshot::Files &files, bool move);
	virtual void done(bool error);
	virtual void canceled();

private:
	IdmContainer m_container;
	IdmEntity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDMCOPYCONTROL_H_ */
