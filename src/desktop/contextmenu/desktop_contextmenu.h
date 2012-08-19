#ifndef DESKTOP_CONTEXTMENU_H_
#define DESKTOP_CONTEXTMENU_H_

#include <QtCore/QCoreApplication>
#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtGui/QMenu>

#include <vfs/actions/vfs_fileaction.h>
#include <vfs/filetypeinfo/vfs_filetypeinfo.h>

#include "../desktop_ns.h"


DESKTOP_NS_BEGIN

class ContextMenu
{
	Q_DECLARE_TR_FUNCTIONS(ContextMenu)

public:
	typedef ::VFS::FileTypeId FileTypeId;
	typedef QList<FileTypeId>        FileTypesList;

public:
	ContextMenu();

	void registerAction(const ::VFS::FileAction *action);

private:
	typedef QList<const ::VFS::FileAction *> FileActionsList;
	typedef QMap<FileTypeId, FileActionsList>       Map;

private:
	QMenu m_menu;
};

DESKTOP_NS_END

#endif /* DESKTOP_CONTEXTMENU_H_ */
