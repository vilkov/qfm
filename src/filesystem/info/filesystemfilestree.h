#ifndef FILESYSTEMFILESTREE_H_
#define FILESYSTEMFILESTREE_H_

#include <QtCore/QMap>
#include <QtCore/QString>
#include "../interfaces/filesystemifilecontainer.h"
#include "../../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

class FilesTree
{
public:
	FilesTree(const IFileContainer *root) :
		m_root(root)
	{}
	~FilesTree();

//	bool open(char *pathAsUtf8, PScopedPointer<IFile> &file, bool createSubfolders, QString &error);

private:
	class Directory;
	typedef QMap<QString, Directory *> Directories;

	class Directory
	{
	public:
		Directory(PScopedPointer<IFileContainer> &control) :
			control(control.take())
		{}
		~Directory();

		IFileContainer *control;
		Directories files;
	};

private:
	const IFileContainer *m_root;
	Directories m_files;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILESTREE_H_ */
