#ifndef IDM_COPYCONTROL_H_
#define IDM_COPYCONTROL_H_

#include <QtCore/QCoreApplication>
#include "../../control/idm_copycontrolbase.h"
#include "../../storage/entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class IdmCopyControl : public IdmCopyControlBase
{
	Q_DECLARE_TR_FUNCTIONS(IdmCopyControl)

public:
	IdmCopyControl(ICopyControl::Holder &dest, const IdmContainer &container, IdmEntity *entity);

	/* ICopyControl */
	virtual bool start(const Snapshot &files, bool move);

private:
	IdmEntity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COPYCONTROL_H_ */
