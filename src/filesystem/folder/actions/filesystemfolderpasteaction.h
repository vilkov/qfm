#ifndef FILESYSTEMFOLDERPASTEACTION_H_
#define FILESYSTEMFOLDERPASTEACTION_H_

#include <QtCore/QCoreApplication>
#include "../../actions/asynchronous/filesystemasyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

class FolderPasteAction : public AsyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderPasteAction)

public:
	FolderPasteAction();

	virtual const QString &lockReason() const;
	virtual PerformActionTask *process(TasksNode *receiver, const FilesList &files) const;

private:
	QString m_lockReason;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERPASTEACTION_H_ */
