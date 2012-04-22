#ifndef DEFAULTFOLDERPASTEACTION_H_
#define DEFAULTFOLDERPASTEACTION_H_

#include <QtCore/QCoreApplication>
#include "../default_ns.h"
#include "../../../filesystem/actions/asynchronous/filesystemasyncfileaction.h"


DEFAULT_PLUGIN_NS_BEGIN

class FolderPasteAction : public AsyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderPasteAction)

public:
	FolderPasteAction();

	virtual const QString &lockReason() const;
	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const;

private:
	QString m_lockReason;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFOLDERPASTEACTION_H_ */
