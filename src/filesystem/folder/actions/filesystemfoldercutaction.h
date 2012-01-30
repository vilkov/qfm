#ifndef FILESYSTEMFOLDERCUTACTION_H_
#define FILESYSTEMFOLDERCUTACTION_H_

#include <QtCore/QCoreApplication>
#include "../../actions/synchronous/filesystemsyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

class FolderCutAction : public SyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderCutAction)

public:
	FolderCutAction();

	virtual const QAction *action() const;
	virtual void process(const FilesList &files) const;

private:
	QAction m_action;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERCUTACTION_H_ */
