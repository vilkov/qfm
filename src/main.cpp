#include "application.h"
#include "tools/memory/memory_manager.h"


#include <iostream>
#include <stlsoft/smartptr/ref_ptr.hpp>
#include <comstl/util/initialisers.hpp>
#include <comstl/util/creation_functions.hpp>
#include <comstl/util/interface_traits.hpp>
#include <comstl/util/variant.hpp>
#include <comstl/string/bstr.hpp>
#include <ShlObj.h>




int main(int argc, char *argv[])
{
	comstl::com_initialiser init;

	IShellFolder *desktop;
	if (SUCCEEDED(SHGetDesktopFolder(&desktop)))
	{
		ITEMIDLIST *pidl;
		wchar_t str[] = L"f:\\workspace\\executables\\qfm";

		if (SUCCEEDED(desktop->ParseDisplayName(NULL, NULL, str, NULL, &pidl, NULL)))
		{
			SHCreateDefaultContextMenu();


			CoTaskMemFree(pidl);
			std::cout << "123123123123123" << std::endl;
		}

		desktop->Release();
	}



	MemoryManager manager(4096, 64, 4);
	return Application(QString::fromLatin1("qfm"), QString::fromLatin1("Qt based file manager"), QString(), argc, argv).exec();
}
