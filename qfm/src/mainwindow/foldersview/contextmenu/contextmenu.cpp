#include "contextmenu.h"
#ifdef Q_WS_WIN
//#	include "win/contextmenu_win.h"
#endif
#include <QtGui/QCursor>


ContextMenu::ContextMenu(QWidget *parent) :
#ifdef Q_WS_WIN
	m_data()//new ContextMenuWin(parent))
#else
	m_data()
#endif
{}

void ContextMenu::popup(const QString &parentDir, const QStringList &files)
{
	m_data->popup(parentDir, files, QCursor::pos());
}
