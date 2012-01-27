#ifndef DECONTEXTMENU_H_
#define DECONTEXTMENU_H_

#include <QtCore/QCoreApplication>
#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtGui/QMenu>
#include "../de_ns.h"
#include "../../filesystem/actions/filesystemfileaction.h"
#include "../../filesystem/filetypeinfo/filetypeinfo.h"


DE_NS_BEGIN

class ContextMenu
{
	Q_DECLARE_TR_FUNCTIONS(ContextMenu)

public:
	typedef ::FileSystem::FileTypeId FileTypeId;
	typedef QList<FileTypeId>        FileTypesList;

public:
	ContextMenu();

	void registerAction(const ::FileSystem::FileAction *action);

private:
	typedef QList<const ::FileSystem::FileAction *> FileActionsList;
	typedef QMap<FileTypeId, FileActionsList>       Map;

private:
	QMenu m_menu;
};

DE_NS_END

#endif /* DECONTEXTMENU_H_ */
