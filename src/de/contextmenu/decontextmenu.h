#ifndef DECONTEXTMENU_H_
#define DECONTEXTMENU_H_

#include <QtCore/QCoreApplication>
#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtGui/QMenu>
#include "../de_ns.h"
#include "../../filesystem/interfaces/filesysteminode.h"


DE_NS_BEGIN

class ContextMenu
{
	Q_DECLARE_TR_FUNCTIONS(ContextMenu)

public:
	typedef QList< ::FileSystem::IFileInfo *> Files;

public:
	ContextMenu();

	static void popup(QWidget *parent, ::FileSystem::INode *node, const Files &files);

private:
	QMenu m_menu;
};

DE_NS_END

#endif /* DECONTEXTMENU_H_ */
