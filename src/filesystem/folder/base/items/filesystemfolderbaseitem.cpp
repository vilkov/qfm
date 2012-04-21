#include "filesystemfolderbaseitem.h"


FILE_SYSTEM_NS_BEGIN

FolderBaseItem::FolderBaseItem(const Info &info, Base *parent) :
	TaskNodeItem(parent),
	m_node(0),
	m_info(info)
{}

FILE_SYSTEM_NS_END
