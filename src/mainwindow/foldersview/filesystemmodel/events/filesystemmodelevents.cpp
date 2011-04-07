#include "filesystemmodelevents.h"


FileSystemModelBaseEvent::FileSystemModelBaseEvent(EventType type) :
	QEvent((QEvent::Type)type)
{}

ListFilesEvent::ListFilesEvent(const value_type &info) :
	FileSystemModelBaseEvent(NewFileInfo),
	m_info(info)
{}

ChangesListEvent::ChangesListEvent(const value_type &info) :
	FileSystemModelBaseEvent(UpdateFileInfo),
	m_info(info)
{}
