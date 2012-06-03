#ifndef FILESYSTEMFILESTREE_H_
#define FILESYSTEMFILESTREE_H_

#include <QtCore/QMap>
#include <QtCore/QString>
#include "../interfaces/filesystemifilecontainer.h"


FILE_SYSTEM_NS_BEGIN

class FilesTree
{
public:
	FilesTree(const IFileContainer *root, bool createSubfolders) :
		m_root(root),
		m_createSubfolders(createSubfolders)
	{}
	~FilesTree();

	const IFileContainer *open(const QString &filePath, bool isPathToDirectory, QString &error);

private:
	class Directory;
	typedef QMap<QString, Directory *> Directories;

	class Directory
	{
	public:
		Directory(IFileContainer::Holder &control) :
			control(control.take())
		{}
		~Directory();

		IFileContainer::Holder control;
		Directories files;
	};

private:
	inline const QChar *strchr(const QChar *string, QChar c) const;

private:
	const IFileContainer *m_root;
	bool m_createSubfolders;
	Directories m_files;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILESTREE_H_ */
