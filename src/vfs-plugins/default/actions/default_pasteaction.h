#ifndef DEFAULT_PASTEACTION_H_
#define DEFAULT_PASTEACTION_H_

#include <QtCore/QCoreApplication>
#include <vfs/actions/asynchronous/vfs_asyncfileaction.h>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class PasteAction : public AsyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(PasteAction)

public:
	PasteAction();

	virtual const QString &lockReason() const;
	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const;

private:
	QString m_lockReason;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_PASTEACTION_H_ */
