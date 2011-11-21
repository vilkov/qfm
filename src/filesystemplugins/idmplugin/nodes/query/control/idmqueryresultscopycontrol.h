#ifndef IDMQUERYRESULTSCOPYCONTROL_H_
#define IDMQUERYRESULTSCOPYCONTROL_H_

#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include "../idmplugin_ns.h"
#include "../containeres/idmcontainer.h"
#include "../storage/entities/idmentity.h"
#include "../../../filesystem/info/filesystemcopyinfo.h"


IDM_PLUGIN_NS_BEGIN

class IdmQueryResultsCopyControl : public CopyInfo
{
	Q_DECLARE_TR_FUNCTIONS(IdmCopyControl)

public:
	IdmQueryResultsCopyControl(const IdmContainer &container, IdmEntity *entity, const Info &info);

	/* ICopyControl */
	virtual bool start(const InfoListItem *files, bool move);
	virtual void done(bool error);
	virtual void canceled();

private:
	QStringList toStringList(const InfoListItem *files) const;

private:
	IdmContainer m_container;
	IdmEntity *m_entity;
};

IDM_PLUGIN_NS_END

#endif /* IDMQUERYRESULTSCOPYCONTROL_H_ */
