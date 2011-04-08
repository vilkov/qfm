#include "filesystemmodelevents.h"


FileSystemModelBaseEvent::FileSystemModelBaseEvent(FileSystemTree *fileSystemTree, EventType type) :
	QEvent((QEvent::Type)type),
	m_fileSystemTree(fileSystemTree)
{}

ListFilesEvent::ListFilesEvent(FileSystemTree *fileSystemTree, const value_type &info) :
	FileSystemModelBaseEvent(fileSystemTree, NewFileInfo),
	m_info(info)
{}

ChangesListEvent::ChangesListEvent(FileSystemTree *fileSystemTree, const value_type &info) :
	FileSystemModelBaseEvent(fileSystemTree, UpdateFileInfo),
	m_info(info)
{}
