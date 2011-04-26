#ifndef CONTEXTMENU_WIN_H_
#define CONTEXTMENU_WIN_H_

#include <QtCore/qt_windows.h>
#include <QtCore/QVector>
#include <stlsoft/smartptr/ref_ptr.hpp>
#include <comstl/util/initialisers.hpp>
#include <comstl/util/creation_functions.hpp>
#include <comstl/util/interface_traits.hpp>
#include <comstl/util/variant.hpp>
#include <comstl/string/bstr.hpp>
#include <ShlObj.h>

#include <QtGui/QMenu>
#include "../contextmenu.h"


namespace stlsoft
{
	namespace comstl_project
	{
		COMSTL_IID_TRAITS_DEFINE(IShellFolder)
		COMSTL_IID_TRAITS_DEFINE(IContextMenu2)
		COMSTL_IID_TRAITS_DEFINE(IContextMenu3)
	}
}


class ContextMenuWin : public ContextMenu::Implementation
{
public:
	ContextMenuWin(QWidget *parent);

	virtual void popup(const QString &parentDir, const QStringList &files, const QPoint &pos);

private:
	class ItemIdListStorage
	{
	public:
		typedef const ITEMIDLIST *      value_type;
		typedef QVector<value_type>     storage_type;
		typedef storage_type::size_type size_type;

	public:
		ItemIdListStorage()
		{}
		~ItemIdListStorage()
		{
			for (size_type i = 0, size = m_data.size(); i < size; ++i)
				CoTaskMemFree((void*)m_data.at(i));
		}

		inline size_type size() const { return m_data.size(); }
		inline bool isEmpty() const { return m_data.isEmpty(); }
		inline void push_back(value_type value) { m_data.push_back(value); }
	    inline value_type *data() { return m_data.data(); }

	private:
	    storage_type m_data;
	};

private:
    enum { BufferSize = 256 };
    void populateMenu(HMENU hmenu);
    void populateMenu(QMenu *menu, HMENU hmenu, MENUITEMINFO &info, char *buffer);

private:
    static bool eventFilter(MSG *message, long *result);

private:
	QMenu m_menu;
	comstl::com_initialiser m_init;
	stlsoft::ref_ptr<IShellFolder> m_desktop;
	stlsoft::ref_ptr<IContextMenu2> m_menu2;
	stlsoft::ref_ptr<IContextMenu3> m_menu3;
};

#endif /* CONTEXTMENU_WIN_H_ */
