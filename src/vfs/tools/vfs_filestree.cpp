#include "vfs_filestree.h"


VFS_NS_BEGIN

FilesTree::~FilesTree()
{
	qDeleteAll(m_files);
}

const IFileContainer *FilesTree::open(const QString &filePath, bool isPathToDirectory, QString &error)
{
	const IFileContainer *container;
	const QChar *path = filePath.constData();
	const QChar *sep;

	if ((sep = strchr(path, L'/')) != NULL)
	{
		QString fileName;

		Directory *ptr;
		Directory *&folder = m_files[fileName = QString(path, sep - path)];

		if (folder == NULL)
		{
			IFileContainer::Holder container(m_root->create(fileName, error));

			if (container)
				folder = ptr = new Directory(container);
			else
				return NULL;
		}
		else
			ptr = folder;

		path = (++sep);

		while ((sep = strchr(path, L'/')) != NULL)
		{
			Directory *&subfolder = ptr->files[fileName = QString(path, sep - path)];

			if (subfolder == NULL)
			{
				IFileContainer::Holder container(ptr->control->create(fileName, error));

				if (container)
					subfolder = ptr = new Directory(container);
				else
					return NULL;
			}
			else
				ptr = subfolder;

			path = (++sep);
		}

		if (isPathToDirectory && !(fileName = QString(path)).isEmpty())
		{
			Directory *&subfolder = ptr->files[fileName];

			if (subfolder == NULL)
			{
				IFileContainer::Holder container(ptr->control->create(fileName, error));

				if (container)
					subfolder = ptr = new Directory(container);
				else
					return NULL;
			}
			else
				ptr = subfolder;
		}

		container = ptr->control.data();
	}
	else
		if (isPathToDirectory && !filePath.isEmpty())
		{
			Directory *&folder = m_files[filePath];

			if (folder == NULL)
			{
				IFileContainer::Holder container(m_root->create(filePath, error));

				if (container)
					folder = new Directory(container);
				else
					return NULL;
			}

			container = folder->control.data();
		}
		else
			container = m_root;

	return container;
}

FilesTree::Directory::~Directory()
{
	qDeleteAll(files);
}

const QChar *FilesTree::strchr(const QChar *string, QChar c) const
{
	while ((*string) != 0)
		if ((*string) == c)
			return string;
		else
			++string;

	return NULL;
}

VFS_NS_END
