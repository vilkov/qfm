#ifndef IDMQUERYRESULTSCOPYCONTROL_H_
#define IDMQUERYRESULTSCOPYCONTROL_H_

#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include "../interface/idminodequeryresults.h"
#include "../../../containeres/idmcontainer.h"
#include "../../../storage/values/idmentityvalue.h"
#include "../../../../../filesystem/info/filesystemcopyinfo.h"


IDM_PLUGIN_NS_BEGIN

class IdmQueryResultsCopyControl : public CopyInfo
{
	Q_DECLARE_TR_FUNCTIONS(IdmCopyControl)

public:
	IdmQueryResultsCopyControl(const IdmContainer &container, IdmCompositeEntityValue *value, const IdmEntity::Property &property, IQueryResultsUpdater *model, const QModelIndex &index, const Info &info);

	/* ICopyControl */
	virtual bool start(const InfoListItem *files, bool move);
	virtual void done(bool error);
	virtual void canceled();

private:
	QStringList toStringList(const InfoListItem *files) const;

private:
	IdmContainer m_container;
	IdmCompositeEntityValue *m_value;
	const IdmEntity::Property &m_property;
	IQueryResultsUpdater *m_model;
	QModelIndex m_index;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTSCOPYCONTROL_H_ */
