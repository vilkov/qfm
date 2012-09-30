#ifndef IDM_COPYCONTROL_H_
#define IDM_COPYCONTROL_H_

#include <QtCore/QCoreApplication>
#include "../../control/idm_copycontrolbase.h"
#include "../../storage/entities/idm_entity.h"


IDM_PLUGIN_NS_BEGIN

class CopyControl : public CopyControlBase
{
	Q_DECLARE_TR_FUNCTIONS(CopyControl)

public:
	CopyControl(ICopyControl::Holder &dest, const IdmContainer &container, Entity *entity);

	/* ICopyControl */
	virtual bool start(const Snapshot &files, bool move);

private:
	Entity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COPYCONTROL_H_ */
