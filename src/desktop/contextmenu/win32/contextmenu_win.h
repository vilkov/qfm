/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CONTEXTMENU_WIN_H_
#define CONTEXTMENU_WIN_H_

#include <QtCore/qt_windows.h>
#include <QtCore/QByteArray>
#include <QtCore/QMap>
#include <QtGui/QMenu>
#include "../contextmenu.h"

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
#include <ShlObj.h>


namespace stlsoft
{
	namespace comstl_project
	{
		COMSTL_IID_TRAITS_DEFINE(IShellFolder)
		COMSTL_IID_TRAITS_DEFINE(IContextMenu)
		COMSTL_IID_TRAITS_DEFINE(IContextMenu2)
		COMSTL_IID_TRAITS_DEFINE(IContextMenu3)
	}
}


/* TODO: Menu icons is not drawn for all elements (Tortoise SVN) */
class ContextMenuWin : public ContextMenu::Implementation
{
public:
	ContextMenuWin(QWidget *parent);

	virtual void popup(const QString &parentDir, const QStringList &files, const QPoint &pos);

private:
	class ItemIdListStorage
	{
	public:
		typedef const ITEMIDLIST * value_type;
		typedef value_type       * storage_type;
		typedef int                size_type;
		enum { Bof = (size_type)-1 };

	public:
		ItemIdListStorage(size_type size) :
			m_data(static_cast<storage_type>(CoTaskMemAlloc(size * sizeof(value_type)))),
			m_index(Bof)
		{
			Q_ASSERT(size > 0);
		}
		~ItemIdListStorage()
		{
			for (size_type i = 0, sz = size(); i < sz; ++i)
				CoTaskMemFree((void*)m_data[i]);

			CoTaskMemFree(m_data);
		}

		inline size_type size() const { return m_index + 1; }
		inline bool isEmpty() const { return m_index == Bof; }
		inline void push_back(value_type value) { m_data[++m_index] = value; }
	    inline storage_type data() { return m_data; }

	private:
	    storage_type m_data;
	    size_type m_index;
	};

private:
    typedef QPair<QByteArray, UINT> Command;
    enum { BufferSize = 256 };
    void populateMenu(HMENU hmenu);
    void populateMenu(QMenu *menu, HMENU hmenu, MENUITEMINFO &info, char *buffer);
    void invokeCommand(const Command &command);

//private:
//    static bool eventFilter(MSG *message, long *result);

private:
	QMenu m_menu;
	comstl::com_initialiser m_init;
	stlsoft::ref_ptr<IShellFolder> m_desktop;
	stlsoft::ref_ptr<IContextMenu> m_menu0;
	stlsoft::ref_ptr<IContextMenu2> m_menu2;
	stlsoft::ref_ptr<IContextMenu3> m_menu3;
	QMap<QAction*, Command> m_commands;
};

#endif /* CONTEXTMENU_WIN_H_ */
