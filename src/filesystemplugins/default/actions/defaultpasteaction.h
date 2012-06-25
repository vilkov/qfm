#ifndef DEFAULTPASTEACTION_H_
#define DEFAULTPASTEACTION_H_

#include <QtCore/QCoreApplication>
#include "../default_ns.h"
#include "../../../filesystem/actions/asynchronous/filesystemasyncfileaction.h"


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

#endif /* DEFAULTPASTEACTION_H_ */
