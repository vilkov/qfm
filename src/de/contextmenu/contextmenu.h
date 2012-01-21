#ifndef CONTEXTMENU_H_
#define CONTEXTMENU_H_

#include <QtCore/QCoreApplication>
#include <QtCore/QPoint>
#include <QtGui/QMenu>


class ContextMenu
{
	Q_DECLARE_TR_FUNCTIONS(ContextMenu)

public:
	ContextMenu();

private:
	QMenu m_menu;
};

#endif /* CONTEXTMENU_H_ */
