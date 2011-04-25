#include "contextmenu.h"
#include <QtCore/QDir>
#include <QtGui/QCursor>

#ifdef Q_OS_WIN32
#	include <stlsoft/smartptr/ref_ptr.hpp>
#	include <comstl/util/initialisers.hpp>
#	include <comstl/util/creation_functions.hpp>
#	include <comstl/util/interface_traits.hpp>
#	include <comstl/util/variant.hpp>
#	include <comstl/string/bstr.hpp>
#	include <ShlObj.h>

	namespace stlsoft
	{
		namespace comstl_project
		{
			COMSTL_IID_TRAITS_DEFINE(IShellFolder)
			COMSTL_IID_TRAITS_DEFINE(IContextMenu)
		}
	}
#endif


ContextMenu::ContextMenu() :
	m_menu(0)
{}

ContextMenu::ContextMenu(const FileSystemInfo &info) :
	m_menu(new QMenu())
{
#ifdef Q_OS_WIN32
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
				HRESULT res;
				IContextMenu *iMenu;
				const ITEMIDLIST *files[1] = { pidl };

				if (SUCCEEDED(res = desktop->GetUIObjectOf(NULL, 1, files, IID_IContextMenu, NULL, (void**)&iMenu)))
				{
					stlsoft::ref_ptr<IContextMenu> menu(iMenu, false);
					HMENU hmenu = CreatePopupMenu();

					if (SUCCEEDED(res = menu->QueryContextMenu(hmenu, 0, 0, 1024, CMF_NORMAL)))
					{
						char title[1024];
						MENUITEMINFO info;

						memset(&info, 0, sizeof(MENUITEMINFO));
						info.cbSize = sizeof(MENUITEMINFO);
						info.fMask = MIIM_STRING;
						info.dwTypeData = (wchar_t*)title;
						info.cch = 1024;

						for (int i = 0, size = GetMenuItemCount(hmenu); i < size; ++i)
						{
							memset(&title, 0, 1024);
							info.cch = 1024;

							if (GetMenuItemInfo(hmenu, i, true, &info))
								m_menu->addAction(QString::fromUtf16((const ushort*)info.dwTypeData, info.cch));
						}

						DestroyMenu(hmenu);
					}
				}

				CoTaskMemFree(pidl);
			}
		}
	}
#endif
}

ContextMenu::~ContextMenu()
{
	delete m_menu;
}

void ContextMenu::popup()
{
	m_menu->exec(QCursor::pos());
}
