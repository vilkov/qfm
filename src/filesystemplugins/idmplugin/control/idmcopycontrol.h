#ifndef IDMCOPYCONTROL_H_
#define IDMCOPYCONTROL_H_

#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include "../containeres/idmcontainer.h"
#include "../storage/entities/idmentity.h"
#include "../../../filesystem/interfaces/imp/filesystemcopycontrol.h"


IDM_PLUGIN_NS_BEGIN

class IdmCopyControl : public CopyControl
{
	Q_DECLARE_TR_FUNCTIONS(IdmCopyControl)

public:
	IdmCopyControl(INode *node, const IdmContainer &container, const IFileContainer *folder, IdmEntity *entity);

	/* ICopyControl */
	virtual bool start(const Snapshot::Files &files, bool move);
	virtual void done(bool error);
	virtual void canceled();

private:
	static QString difference(const QString &path1, const QString &path2);

private:
	IdmContainer m_container;
	IdmEntity *m_entity;
	QString m_storage;
};

IDM_PLUGIN_NS_END

#endif /* IDMCOPYCONTROL_H_ */
