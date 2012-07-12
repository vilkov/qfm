#include "../decontextmenufactory.h"


DE_NS_BEGIN

ContextMenuFactory::ContextMenuFactory()
{}

void ContextMenuFactory::registerAction(const ::VFS::FileAction *action, Type type)
{
	m_actions[type].push_back(action);
}

void ContextMenuFactory::registerAction(const ::VFS::FileAction *action, const FileTypesList &fileTypes)
{
	for (FileTypesList::size_type i = 0, size = fileTypes.size(); i < size; ++i)
		m_files[fileTypes.at(i)].push_back(action);
}

DE_NS_END
