#include "filesystemmodelevents.h"


FileSystemModelBaseEvent::FileSystemModelBaseEvent(FileSystemTree *fileSystemTree, EventType type) :
	QEvent((QEvent::Type)type),
	m_fileSystemTree(fileSystemTree)
{}

ListFilesEvent::ListFilesEvent(FileSystemTree *fileSystemTree, const value_type &info) :
	FileSystemModelBaseEvent(fileSystemTree, ListFilesType),
	m_info(info)
{}

ChangesListEvent::ChangesListEvent(FileSystemTree *fileSystemTree, const value_type &info) :
	FileSystemModelBaseEvent(fileSystemTree, ChangesListType),
	m_info(info)
{}

PopulateFilesForRemoveEvent::PopulateFilesForRemoveEvent(FileSystemTree *fileSystemTree, FileSystemTree *subtree) :
	FileSystemModelBaseEvent(fileSystemTree, PopulateFilesForRemoveType),
	m_subtree(subtree)
{}
