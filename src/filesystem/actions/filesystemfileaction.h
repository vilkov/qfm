#ifndef FILESYSTEMFILEACTION_H_
#define FILESYSTEMFILEACTION_H_

#include <QtCore/QList>
#include <QtGui/QAction>
#include "../model/items/filesystemitem.h"
#include "../interfaces/filesystemifilecontrol.h"


FILE_SYSTEM_NS_BEGIN

class FileAction
{
public:
	typedef QPair<const FileSystemItem *, const IFileControl *> FileItem;
	typedef QList<FileItem>                                     FilesList;

public:
	virtual ~FileAction();

	virtual bool isAsynchronous() const = 0;
	virtual const QAction *action() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILEACTION_H_ */
