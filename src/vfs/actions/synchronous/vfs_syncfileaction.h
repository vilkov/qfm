#ifndef VFS_SYNCFILEACTION_H_
#define VFS_SYNCFILEACTION_H_

#include "../vfs_fileaction.h"


VFS_NS_BEGIN

class SyncFileAction : public FileAction
{
public:
	SyncFileAction(const QIcon &icon, const QString &text);

	virtual bool isAsynchronous() const;
	virtual void process(const IFileContainer *container, const FilesList &files) const = 0;
};

VFS_NS_END

#endif /* VFS_SYNCFILEACTION_H_ */
