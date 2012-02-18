#ifndef FILESYSTEMFILESTREE_H_
#define FILESYSTEMFILESTREE_H_

#include <QtCore/QMap>
#include <QtCore/QString>
#include "../interfaces/filesystemifilecontrol.h"
#include "../../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

class FilesTree
{
public:
	FilesTree(const IFileControl *root) :
		m_root(root)
	{}
	~FilesTree();

	const IFileControl *open(char *pathAsUtf8, bool createSubfolders, QString &error);

private:
	class File;
	typedef QMap<QString, File *> Files;

	class File
	{
	public:
		File(PScopedPointer<IFileControl> &control) :
			control(control.take())
		{}
		~File();

		IFileControl *control;
		Files files;
	};

private:
	const IFileControl *m_root;
	Files m_files;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILESTREE_H_ */
