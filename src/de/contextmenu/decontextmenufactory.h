#ifndef DECONTEXTMENUFACTORY_H_
#define DECONTEXTMENUFACTORY_H_

#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtGui/QMenu>

#include <vfs/actions/vfs_fileaction.h>
#include <vfs/filetypeinfo/vfs_filetypeinfo.h>

#include "../de_ns.h"


DE_NS_BEGIN

class ContextMenuFactory
{
public:
	typedef ::VFS::FileTypeId                FileTypeId;
	typedef QList<FileTypeId>                FileTypesList;
	typedef QList<const ::VFS::FileAction *> FileActionsList;

	enum Type
	{
		SingleFolder,
		SingleFile,
		MultipleFilesOrFolders,
		AnyFilesOrFolders,

		/* Internal use only! */
		TypeCount
	};

public:
	ContextMenuFactory();

	FileActionsList actions(Type type) { return m_actions[type]; }
	FileActionsList actions(const FileTypeId &type) { return m_files.value(type); }

	void registerAction(const ::VFS::FileAction *action, Type type);
	void registerAction(const ::VFS::FileAction *action, const FileTypesList &fileTypes);

private:
	typedef QMap<FileTypeId, FileActionsList> FileActionsMap;

private:
	FileActionsMap m_files;
	FileActionsList m_actions[TypeCount];
};

DE_NS_END

#endif /* DECONTEXTMENUFACTORY_H_ */
