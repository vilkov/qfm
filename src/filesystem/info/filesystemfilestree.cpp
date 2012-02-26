#include "filesystemfilestree.h"
#include <string.h>


FILE_SYSTEM_NS_BEGIN

FilesTree::~FilesTree()
{
	qDeleteAll(m_files);
}

bool FilesTree::open(char *pathAsUtf8, PScopedPointer<IFile> &file, bool createSubfolders, QString &error)
{
	QString fileName;
    char *sep;

	if ((sep = strchr(pathAsUtf8, '/')) != NULL)
	{
		(*sep) = 0;
		Directory *ptr;
		Directory *&folder = m_files[fileName = QString::fromUtf8(pathAsUtf8)];

		if (folder == NULL)
		{
			PScopedPointer<IFileContainer> container(m_root->open(fileName, createSubfolders, error));

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
				PScopedPointer<IFileContainer> container(ptr->control->open(fileName, createSubfolders, error));

				if (container)
					subfolder = ptr = new Directory(container);
				else
					return false;
			}
			else
				ptr = subfolder;

			pathAsUtf8 = (++sep);
		}

		if (!(fileName = QString::fromUtf8(pathAsUtf8)).isEmpty())
			file = ptr->control->open(fileName);
	}
	else
		if (!(fileName = QString::fromUtf8(pathAsUtf8)).isEmpty())
			file = m_root->open(fileName);

	return true;
}

FilesTree::Directory::~Directory()
{
	qDeleteAll(files);
	delete control;
}

FILE_SYSTEM_NS_END
