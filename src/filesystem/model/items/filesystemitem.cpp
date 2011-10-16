#include "filesystemitem.h"


FILE_SYSTEM_NS_BEGIN

FileSystemItem::FileSystemItem(const Node::Holder &node, Item *parent) :
	Item(parent),
	m_node(node)
{}

bool FileSystemItem::isList() const
{
	return false;
}

FILE_SYSTEM_NS_END
