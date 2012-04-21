#include "filesystemfilestree.h"
#include <string.h>


FILE_SYSTEM_NS_BEGIN

FilesTree::~FilesTree()
{
	qDeleteAll(m_files);
}

bool FilesTree::open(char *pathAsUtf8, bool createSubfolders, const IFileContainer *&container, QString &fileName, QString &error)
{
    char *sep;

	if ((sep = strchr(pathAsUtf8, '/')) != NULL)
	{
		(*sep) = 0;
		Directory *ptr;
		Directory *&folder = m_files[fileName = QString::fromUtf8(pathAsUtf8)];

		if (folder == NULL)
		{
			IFileContainer::Holder container(m_root->open(fileName, createSubfolders, error));

			if (container)
				folder = ptr = new Directory(container);
			else
				return false;
		}
		else
			ptr = folder;

		pathAsUtf8 = (++sep);

		while ((sep = strchr(pathAsUtf8, '/')) != NULL)
		{
			(*sep) = 0;
			Directory *&subfolder = ptr->files[fileName = QString::fromUtf8(pathAsUtf8)];

			if (subfolder == NULL)
			{
				IFileContainer::Holder container(ptr->control->open(fileName, createSubfolders, error));

				if (container)
					subfolder = ptr = new Directory(container);
				else
					return false;
			}
			else
				ptr = subfolder;

			pathAsUtf8 = (++sep);
		}

		fileName = QString::fromUtf8(pathAsUtf8);
		container = ptr->control.data();
	}
	else
	{
		container = m_root;
		fileName = QString::fromUtf8(pathAsUtf8);
	}

	return true;
}

FilesTree::Directory::~Directory()
{
	qDeleteAll(files);
}

FILE_SYSTEM_NS_END
