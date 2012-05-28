#ifndef FILESYSTEMFILEACTION_H_
#define FILESYSTEMFILEACTION_H_

#include <QtCore/QList>
#include <QtGui/QAction>
#include "../model/items/filesystemnodeitem.h"
#include "../interfaces/filesystemifilecontainer.h"


FILE_SYSTEM_NS_BEGIN

class FileAction
{
public:
	typedef QPair<NodeItem::Holder, const IFileInfo *> FileItem;
	typedef QList<FileItem>                            FilesList;

public:
	FileAction(const QIcon &icon, const QString &text);
	virtual ~FileAction();

	const QAction *action() const { return &m_action; }

	static const FileAction *fromQAction(const QAction *action);
	static FileAction *fromQAction(QAction *action);

	virtual bool isAsynchronous() const = 0;

private:
	QAction m_action;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILEACTION_H_ */
