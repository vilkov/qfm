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
	typedef ::FileSystem::FileTypeId FileTypeId;
	typedef QList<FileTypeId>        FileTypesList;

	enum Type
	{
		SingleFolder,
		SingleFile,
		MultipleFilesOrFolders,

		/* Internal use only! */
		TypeCount
	};

public:
	ContextMenuFactory();

	void registerAction(const ::FileSystem::FileAction *action, Type type);
	void registerAction(const ::FileSystem::FileAction *action, const FileTypesList &fileTypes);

private:
	typedef QList<const ::FileSystem::FileAction *> FileActionsList;
	typedef QMap<FileTypeId, FileActionsList>       FileActionsMap;

private:
	FileActionsMap m_files;
	FileActionsList m_actions[TypeCount];
};

DE_NS_END

#endif /* DECONTEXTMENUFACTORY_H_ */
