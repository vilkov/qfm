#ifndef FILESYSTEMFOLDERPASTEINTOFOLDERACTION_H_
#define FILESYSTEMFOLDERPASTEINTOFOLDERACTION_H_

#include <QtCore/QCoreApplication>
#include "../../actions/asynchronous/filesystemasyncfileaction.h"


FILE_SYSTEM_NS_BEGIN

class FolderPasteIntoFolderAction : public AsyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(FolderPasteIntoFolderAction)

public:
	FolderPasteIntoFolderAction();

	virtual const QString &lockReason() const;
	virtual bool prepare(const FilesList &files);
	virtual void process(PerformActionTask *task, const FilesList &files, const volatile Flags &aborted) const;

private:
	QString m_lockReason;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERPASTEINTOFOLDERACTION_H_ */
