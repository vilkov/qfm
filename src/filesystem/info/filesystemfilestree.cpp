#include "filesystemfilestree.h"
#include <string.h>


FILE_SYSTEM_NS_BEGIN

FilesTree::~FilesTree()
{
	qDeleteAll(m_files);
}

const IFileControl *FilesTree::open(char *pathAsUtf8, bool createSubfolders, QString &error)
{
	QString fileName;
    char *sep;

	if ((sep = strchr(pathAsUtf8, '/')) != NULL)
	{
		(*sep) = 0;
		File *ptr;
		File *&folder = m_files[fileName = QString::fromUtf8(pathAsUtf8)];

		if (folder == NULL)
		{
			PScopedPointer<IFileControl> container(m_root->openFolder(fileName, createSubfolders, error));

			if (container)
				folder = ptr = new File(container);
			else
				return NULL;
		}
		else
			ptr = folder;

		pathAsUtf8 = (++sep);

		while ((sep = strchr(pathAsUtf8, '/')) != NULL)
		{
			(*sep) = 0;
			File *&subfolder = ptr->files[fileName = QString::fromUtf8(pathAsUtf8)];

			if (subfolder == NULL)
			{
				PScopedPointer<IFileControl> container(ptr->control->openFolder(fileName, createSubfolders, error));

				if (container)
					subfolder = ptr = new File(container);
				else
					return NULL;
			}
			else
				ptr = subfolder;

			pathAsUtf8 = (++sep);
		}

		if (!(fileName = QString::fromUtf8(pathAsUtf8)).isEmpty())
		{
			File *&file = ptr->files[fileName];

			if (file == NULL)
			{
				PScopedPointer<IFileControl> container(ptr->control->openFile(fileName, error));

				if (container)
					file = ptr = new File(container);
				else
					return NULL;
			}
			else
				ptr = file;
		}

		return ptr->control;
	}
	else
	{
		File *&file = m_files[fileName = QString::fromUtf8(pathAsUtf8)];

		if (file == NULL)
		{
			PScopedPointer<IFileControl> container(m_root->openFile(fileName, error));

			if (container)
				return (file = new File(container))->control;
			else
				return NULL;
		}
		else
			return file->control;
	}

	return 0;
}

FilesTree::File::~File()
{
	qDeleteAll(files);
	delete control;
}

FILE_SYSTEM_NS_END
