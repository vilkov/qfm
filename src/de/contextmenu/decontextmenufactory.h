#ifndef DECONTEXTMENUFACTORY_H_
#define DECONTEXTMENUFACTORY_H_

#include <QtCore/QPoint>
#include <QtCore/QList>
#include <QtGui/QMenu>
#include "../de_ns.h"
#include "../../filesystem/actions/filesystemfileaction.h"
#include "../../filesystem/filetypeinfo/filetypeinfo.h"


DE_NS_BEGIN

class ContextMenuFactory
{
public:
	typedef ::FileSystem::FileTypeId                FileTypeId;
	typedef QList<FileTypeId>                       FileTypesList;
	typedef QList<const ::FileSystem::FileAction *> FileActionsList;

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

	void registerAction(const ::FileSystem::FileAction *action, Type type);
	void registerAction(const ::FileSystem::FileAction *action, const FileTypesList &fileTypes);

private:
	typedef QMap<FileTypeId, FileActionsList> FileActionsMap;

private:
	FileActionsMap m_files;
	FileActionsList m_actions[TypeCount];
};

DE_NS_END

#endif /* DECONTEXTMENUFACTORY_H_ */
