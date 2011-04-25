#ifndef CONTEXTMENU_H_
#define CONTEXTMENU_H_

#include <QtGui/QMenu>
#include "../filesysteminfo.h"


class ContextMenu
{
public:
	ContextMenu();
	ContextMenu(const FileSystemInfo &info);
	~ContextMenu();

    void popup();

private:
	QMenu *m_menu;
};

#endif /* CONTEXTMENU_H_ */
