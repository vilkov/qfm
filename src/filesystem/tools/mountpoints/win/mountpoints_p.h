#ifndef MOUNTPOINTS_P_H_
#define MOUNTPOINTS_P_H_

#include "../mountpoint.h"

#include <stlsoft/stlsoft.h>
#if defined(STLSOFT_CF_EXCEPTION_SUPPORT)
#	undef STLSOFT_CF_EXCEPTION_SUPPORT
#endif
#include <stlsoft/smartptr/ref_ptr.hpp>
#include <comstl/util/initialisers.hpp>
#include <comstl/util/creation_functions.hpp>
#include <comstl/util/interface_traits.hpp>
#include <comstl/util/variant.hpp>
#include <comstl/string/bstr.hpp>
#include <shlobj.h>

namespace stlsoft
{
	namespace comstl_project
	{
		COMSTL_IID_TRAITS_DEFINE(IShellFolder)
		COMSTL_IID_TRAITS_DEFINE(IEnumIDList)
		COMSTL_IID_TRAITS_DEFINE(IExtractIcon)
	}
}


class MountPointsPrivate
{
public:
	typedef MountPoint::value_type value_type;
	typedef MountPoint::size_type  size_type;

public:
	MountPointsPrivate();

	void refresh();

	value_type items;
};


MountPointsPrivate::MountPointsPrivate()
{}

void MountPointsPrivate::refresh()
{
	comstl::com_initialiser com;

	if (com.is_initialised())
	{
		IShellFolder *iFolder;

		if (SUCCEEDED(SHGetDesktopFolder(&iFolder)))
		{
			ITEMIDLIST *pidlList;
			stlsoft::ref_ptr<IShellFolder> desktop(iFolder, false);

			if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &pidlList)))
			{
				if (SUCCEEDED(desktop->BindToObject(pidlList, NULL, IID_IShellFolder, (void**)&iFolder)))
				{
					IEnumIDList *iList;
					stlsoft::ref_ptr<IShellFolder> computer(iFolder, false);

					if (SUCCEEDED(computer->EnumObjects(NULL, SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN | SHCONTF_STORAGE, &iList)))
					{
						stlsoft::ref_ptr<IEnumIDList> list(iList, false);
						ITEMIDLIST *pidlList[1] = { static_cast<ITEMIDLIST *>(CoTaskMemAlloc(sizeof(ITEMIDLIST *))) };

						if (pidlList[0])
						{
							ULONG fetched;
							QPixmap pixmap;
							QString label;
							QString path;
							STRRET strret;
							qint32 index;
							IExtractIcon *iIcon;
							wchar_t iconFile[MAX_PATH];
							int iconIndex;
							UINT iconFlags;
							HICON iconHandleLarge;
							HICON iconHandleSmall;
#if defined(STLSOFT_COMPILER_IS_MSVC)
							ULARGE_INTEGER pulFreeBytesAvailableToCaller;
							ULARGE_INTEGER pulTotalNumberOfBytes;
							ULARGE_INTEGER pulTotalNumberOfFreeBytes;
#endif

							items.clear();

							do
								if (FAILED(list->Next(1, pidlList, &fetched)))
									break;
								else
									if (SUCCEEDED(computer->GetDisplayNameOf(pidlList[0], SHGDN_NORMAL, &strret)) && strret.uType == STRRET_WSTR)
									{
										label = QString::fromUtf16((const ushort *)strret.pOleStr);
										CoTaskMemFree(strret.pOleStr);

										if (SUCCEEDED(computer->GetUIObjectOf(NULL, 1, (const ITEMIDLIST **)pidlList, IID_IExtractIcon, NULL, (void**)&iIcon)))
										{
											stlsoft::ref_ptr<IExtractIcon> icon(iIcon, false);

											if (SUCCEEDED(icon->GetIconLocation(GIL_FORSHELL, iconFile, MAX_PATH, &iconIndex, &iconFlags)) &&
												SUCCEEDED(icon->Extract(iconFile, iconIndex, &iconHandleLarge, &iconHandleSmall, 0x00100010)))
											{
												pixmap = QPixmap::fromWinHICON(iconHandleLarge);
											}
											else
												pixmap = QPixmap();
										}
										else
											pixmap = QPixmap();

										if ((index = label.lastIndexOf(QChar(')'))) != -1)
										{
											path = label.mid(index - 2, 2);

#if defined(STLSOFT_COMPILER_IS_MSVC)
											if (SHGetDiskFreeSpaceEx(
													(const wchar_t *)path.unicode(),
													&pulFreeBytesAvailableToCaller,
													&pulTotalNumberOfBytes,
													&pulTotalNumberOfFreeBytes))
												items.push_back(MountPoint(path, label, path, pixmap, pulTotalNumberOfFreeBytes.QuadPart, pulTotalNumberOfBytes.QuadPart));
											else
												items.push_back(MountPoint(path, label, path, pixmap, 0, 0));
#else
											items.push_back(MountPoint(path, label, path, pixmap, 0, 0));
#endif
										}
									}
							while (fetched > 0);

							CoTaskMemFree(pidlList[0]);
						}
					}
				}

				CoTaskMemFree(pidlList);
			}
		}
	}
}

#endif /* MOUNTPOINTS_P_H_ */
