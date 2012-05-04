#ifndef IDMCOPYCONTROL_H_
#define IDMCOPYCONTROL_H_

#include <QtCore/QCoreApplication>
#include "../../control/idmcopycontrolbase.h"
#include "../../storage/entities/idmentity.h"


IDM_PLUGIN_NS_BEGIN

class IdmCopyControl : public IdmCopyControlBase
{
	Q_DECLARE_TR_FUNCTIONS(IdmCopyControl)

public:
	IdmCopyControl(ICopyControl::Holder &dest, const IdmContainer &container, IdmEntity *entity);

	/* ICopyControl */
	virtual bool start(const Snapshot::List &files, bool move);

private:
	IdmEntity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDMCOPYCONTROL_H_ */
