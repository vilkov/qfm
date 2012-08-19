#include "contextmenu_win.h"
//#include "../../../../application.h"
#include <QtCore/QDir>
#include <QtGui/QCursor>
#include <QtGui/QPixmap>
#include <QtGui/QIcon>
#include <QMessageBox>


//static ContextMenuWin *instance;
//static Application::WinEventFilter oldFilter;

ContextMenuWin::ContextMenuWin(QWidget *parent) :
	ContextMenu::Implementation(parent),
	m_desktop(),
	m_menu2(),
	m_menu3()
{
//	instance = this;

	if (m_init.is_initialised())
	{
		IShellFolder *iFolder;

		if (SUCCEEDED(SHGetDesktopFolder(&iFolder)))
			m_desktop.set(iFolder, false);
	}
}

void ContextMenuWin::popup(const QString &parentDir, const QStringList &files, const QPoint &pos)
{
	if (m_init.is_initialised() && !m_desktop.empty())
	{
		HRESULT res;
		ITEMIDLIST *pidl;
		ItemIdListStorage storage(files.size());

		for (QStringList::size_type i = 0, size = files.size(); i < size; ++i)
			if (SUCCEEDED(m_desktop->ParseDisplayName(NULL, NULL, (wchar_t*)QDir::toNativeSeparators(files.at(i)).utf16(), NULL, &pidl, NULL)))
				storage.push_back(pidl);

		if (!storage.isEmpty() &&
			SUCCEEDED(m_desktop->ParseDisplayName(NULL, NULL, (wchar_t*)QDir::toNativeSeparators(parentDir).utf16(), NULL, &pidl, NULL)))
		{
			IShellFolder *iFolder;

			if (SUCCEEDED(m_desktop->BindToObject(pidl, NULL, IID_IShellFolder, (void**)&iFolder)))
			{
				stlsoft::ref_ptr<IShellFolder> folder(iFolder, false);
				IContextMenu *iMenu;

				if (SUCCEEDED(res = folder->GetUIObjectOf(NULL, storage.size(), storage.data(), IID_IContextMenu, NULL, (void**)&iMenu)))
				{					m_menu0.set(iMenu, false);

					if (SUCCEEDED(m_menu0->QueryInterface(IID_IContextMenu2, (void**)&iMenu)))
						m_menu2.set(static_cast<IContextMenu2*>(iMenu), false);

					if (SUCCEEDED(m_menu0->QueryInterface(IID_IContextMenu3, (void**)&iMenu)))
						m_menu3.set(static_cast<IContextMenu3*>(iMenu), false);

					if (HMENU hmenu = CreatePopupMenu())
					{
						if (SUCCEEDED(m_menu0->QueryContextMenu(hmenu, 0, 0, 1024, CMF_NORMAL)))
						{
							populateMenu(hmenu);
							invokeCommand(m_commands.value(m_menu.exec(pos)));
						}
//							{
//								oldFilter = Application::instance()->setWinEventFilter(eventFilter);
//								TrackPopupMenuEx(hmenu, TPM_NONOTIFY | TPM_RETURNCMD | TPM_LEFTBUTTON, pos.x(), pos.y(), parent()->winId(), NULL);
//								Application::instance()->setWinEventFilter(oldFilter);
//							}

						DestroyMenu(hmenu);
					}
				}
				else
					QMessageBox::critical(parent(), QString(), qt_error_string(res));
			}

			CoTaskMemFree(pidl);
		}
	}
}

void ContextMenuWin::populateMenu(HMENU hmenu)
{
	char buffer[BufferSize];
	MENUITEMINFO info;

	memset(&info, 0, sizeof(MENUITEMINFO));
	info.cbSize = sizeof(MENUITEMINFO);
	info.dwTypeData = (wchar_t*)buffer;

	m_menu.clear();
	m_commands.clear();
	populateMenu(&m_menu, hmenu, info, buffer);
}

void ContextMenuWin::populateMenu(QMenu *menu, HMENU hmenu, MENUITEMINFO &info, char *buffer)
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
					info.fMask = MIIM_ID | MIIM_STRING | MIIM_BITMAP | MIIM_SUBMENU | MIIM_CHECKMARKS;
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
						if (info.hbmpChecked != 0)
							icon = QIcon(QPixmap::fromWinHBITMAP((HBITMAP)info.hbmpChecked));
						else
							if (info.hbmpUnchecked != 0)
								icon = QIcon(QPixmap::fromWinHBITMAP((HBITMAP)info.hbmpUnchecked));
							else
								icon = QIcon();

					if (info.hSubMenu != 0)
						populateMenu(menu->addMenu(icon, title), info.hSubMenu, info, buffer);
					else
					{
						memset(buffer, 0, BufferSize);

						if (SUCCEEDED(m_menu0->GetCommandString(info.wID, GCS_VERBA, NULL, buffer, BufferSize)))
							m_commands[menu->addAction(icon, title)] = Command(QByteArray(buffer), info.wID);
						else
							m_commands[menu->addAction(icon, title)] = Command(QByteArray(), info.wID);
					}
				}
	}
}

void ContextMenuWin::invokeCommand(const Command &command)
{
	if (!command.first.isEmpty())
	{
		CMINVOKECOMMANDINFO info;

		memset(&info, 0, sizeof(CMINVOKECOMMANDINFO));
		info.cbSize = sizeof(CMINVOKECOMMANDINFO);
		info.fMask = CMIC_MASK_FLAG_NO_UI;
		info.hwnd = parent()->winId();
		info.lpVerb = command.first.constData();

		m_menu0->InvokeCommand(&info);
	}
	else
		if (command.second != 0)
		{
			CMINVOKECOMMANDINFO info;

			memset(&info, 0, sizeof(CMINVOKECOMMANDINFO));
			info.cbSize = sizeof(CMINVOKECOMMANDINFO);
			info.fMask = CMIC_MASK_FLAG_NO_UI;
			info.hwnd = parent()->winId();
			info.lpVerb = MAKEINTRESOURCEA(command.second);

			m_menu0->InvokeCommand(&info);
		}
}

//bool ContextMenuWin::eventFilter(MSG *msg, long *result)
//{
//	switch (msg->message)
//	{
//		case WM_INITMENUPOPUP:
//		case WM_DRAWITEM:
//		case WM_MENUCHAR:
//		case WM_MEASUREITEM:
//		{
//			if (!instance->m_menu3.empty())
//			{
//				instance->m_menu3->HandleMenuMsg2(msg->message, msg->wParam, msg->lParam, result);
//				return true;
//			}
//
//			if (!instance->m_menu2.empty())
//			{
//				*result = 0;
//				instance->m_menu2->HandleMenuMsg(msg->message, msg->wParam, msg->lParam);
//				return true;
//			}
//
//			break;
//		}
//		default:
//			break;
//	};
//
//	return oldFilter ? (*oldFilter)(msg, result) : false;
//}
