#include "contextmenu.h"
#include <QtCore/QDir>
#include <QtGui/QCursor>
#include <QtGui/QPixmap>
#include <QtGui/QIcon>

#include <stlsoft/smartptr/ref_ptr.hpp>
#include <comstl/util/initialisers.hpp>
#include <comstl/util/creation_functions.hpp>
#include <comstl/util/interface_traits.hpp>
#include <comstl/util/variant.hpp>
#include <comstl/string/bstr.hpp>
#include <ShlObj.h>


#define CMF_EXTENDEDVERBS       0x00000100

namespace stlsoft
{
	namespace comstl_project
	{
		COMSTL_IID_TRAITS_DEFINE(IShellFolder)
		COMSTL_IID_TRAITS_DEFINE(IContextMenu)
	}
}


ContextMenu::ContextMenu() :
	m_data()
{}

ContextMenu::ContextMenu(const FileSystemInfo &info) :
	m_data(new ContextMenuImp())
{
	comstl::com_initialiser init;

	if (init.is_initialised())
	{
		IShellFolder *iDesktop;

		if (SUCCEEDED(SHGetDesktopFolder(&iDesktop)))
		{
			ITEMIDLIST *pidl;
			stlsoft::ref_ptr<IShellFolder> desktop(iDesktop, false);
			QString path = QDir::toNativeSeparators(info.absoluteFilePath());

			if (SUCCEEDED(desktop->ParseDisplayName(NULL, NULL, (wchar_t*)path.constData(), NULL, &pidl, NULL)))
			{
				IContextMenu *iMenu;
				const ITEMIDLIST *files[1] = { pidl };

				if (SUCCEEDED(desktop->GetUIObjectOf(NULL, 1, files, IID_IContextMenu, NULL, (void**)&iMenu)))
				{
					stlsoft::ref_ptr<IContextMenu> menu(iMenu, false);

					if (HMENU hmenu = CreatePopupMenu())
					{
						if (SUCCEEDED(menu->QueryContextMenu(hmenu, 0, 0, 1024, CMF_NORMAL | CMF_EXPLORE | CMF_EXTENDEDVERBS)))
						{
							char buffer[BufferSize];
							MENUITEMINFO info;

							memset(&info, 0, sizeof(MENUITEMINFO));
							info.cbSize = sizeof(MENUITEMINFO);
							info.dwTypeData = (wchar_t*)buffer;

							populateMenu(&m_data->menu, hmenu, info, buffer);
						}

						DestroyMenu(hmenu);
					}
				}

				CoTaskMemFree(pidl);
			}
		}
	}
}

void ContextMenu::popup()
{
	m_data->menu.exec(QCursor::pos());
}

void ContextMenu::populateMenu(QMenu *menu, HMENU hmenu, MENUITEMINFO &info, char *buffer)
{
	QIcon icon;
	QString title;

	for (int i = 0, size = GetMenuItemCount(hmenu); i < size; ++i)
	{
		info.fMask = MIIM_FTYPE;

		if (GetMenuItemInfo(hmenu, i, true, &info))
			if ((info.fType & MFT_SEPARATOR) == MFT_SEPARATOR)
				menu->addSeparator();
			else
				if ((info.fType & MFT_STRING) == MFT_STRING)
				{
					memset(buffer, 0, BufferSize);
					info.fMask = MIIM_STRING | MIIM_BITMAP | MIIM_SUBMENU | MIIM_CHECKMARKS;
					info.cch = BufferSize;

					if (GetMenuItemInfo(hmenu, i, true, &info))
						title = QString::fromUtf16((const ushort*)info.dwTypeData, info.cch);

					if (info.hbmpItem != 0 &&
						info.hbmpItem != HBMMENU_CALLBACK &&
						info.hbmpItem != HBMMENU_MBAR_CLOSE &&
						info.hbmpItem != HBMMENU_MBAR_CLOSE_D &&
						info.hbmpItem != HBMMENU_MBAR_MINIMIZE &&
						info.hbmpItem != HBMMENU_MBAR_MINIMIZE_D &&
						info.hbmpItem != HBMMENU_MBAR_RESTORE &&
						info.hbmpItem != HBMMENU_POPUP_CLOSE &&
						info.hbmpItem != HBMMENU_POPUP_MAXIMIZE &&
						info.hbmpItem != HBMMENU_POPUP_MINIMIZE &&
						info.hbmpItem != HBMMENU_POPUP_RESTORE &&
						info.hbmpItem != HBMMENU_SYSTEM)
						icon = QIcon(QPixmap::fromWinHBITMAP((HBITMAP)info.hbmpItem));
					else
						icon = QIcon();

					if (info.hSubMenu != 0)
						populateMenu(menu->addMenu(icon, title), info.hSubMenu, info, buffer);
					else
						menu->addAction(icon, title);
				}
	}
}
