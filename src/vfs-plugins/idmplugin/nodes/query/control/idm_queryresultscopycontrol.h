#ifndef IDM_QUERYRESULTSCOPYCONTROL_H_
#define IDM_QUERYRESULTSCOPYCONTROL_H_

#include <QtCore/QCoreApplication>
#include "../interface/idm_inodequeryresults.h"
#include "../../../control/idm_copycontrolbase.h"
#include "../../../containeres/idm_container.h"
#include "../../../storage/values/idm_entityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IdmQueryResultsCopyControl : public CopyControlBase
{
	Q_DECLARE_TR_FUNCTIONS(IdmQueryResultsCopyControl)

public:
	IdmQueryResultsCopyControl(ICopyControl::Holder &dest, const IdmContainer &container, const EntityValue::Holder &value, const Entity::Property &property, IQueryResultsUpdater *model, const QModelIndex &index);

	/* ICopyControl */
	virtual bool start(const Snapshot &files, bool move);
	virtual void done(bool error);
	virtual void canceled();

private:
	EntityValue::Holder m_value;
	Entity::Property m_property;
	IQueryResultsUpdater *m_model;
	QModelIndex m_index;
};

IDM_PLUGIN_NS_END

#endif /* IDM_QUERYRESULTSCOPYCONTROL_H_ */
