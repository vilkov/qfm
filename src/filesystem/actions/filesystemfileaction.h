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
	FileAction(const QIcon &icon, const QString &text);
	virtual ~FileAction();

	const QAction *action() const { return &m_action; }

	static const FileAction *fromAction(const QAction *action);
	static FileAction *fromAction(QAction *action);

	virtual bool isAsynchronous() const = 0;

private:
	QAction m_action;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILEACTION_H_ */
