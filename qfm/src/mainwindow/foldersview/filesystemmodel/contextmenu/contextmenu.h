#ifndef CONTEXTMENU_H_
#define CONTEXTMENU_H_

#include <QtCore/qt_windows.h>
#include <QtGui/QMenu>
#include "../filesysteminfo.h"


class ContextMenu
{
public:
	class ContextMenuImp : public QSharedData
	{
		Q_DISABLE_COPY(ContextMenuImp)

	public:
		ContextMenuImp() :
			QSharedData()
		{}

		QMenu menu;
	};

public:
	ContextMenu();
	ContextMenu(const FileSystemInfo &info);

    void popup();

private:
    enum
    {
    	BufferSize = 1024
    };
    void populateMenu(QMenu *menu, HMENU hmenu, MENUITEMINFO &info, char *buffer);

private:
    QExplicitlySharedDataPointer<ContextMenuImp> m_data;
};

#endif /* CONTEXTMENU_H_ */
