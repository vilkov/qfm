#ifndef DEFAULTOPENWITHACTION_H_
#define DEFAULTOPENWITHACTION_H_

#include <QtCore/QCoreApplication>
#include <vfs/interfaces/vfs_iapplication.h>
#include <vfs/actions/synchronous/vfs_syncfileaction.h>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class OpenWithAction : public SyncFileAction
{
public:
	OpenWithAction(const IApplication *application);

	virtual void process(const IFileContainer *container, const FilesList &files) const;

protected:
	static QString label(const IApplication *application);

private:
	const IApplication *m_application;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTOPENWITHACTION_H_ */
