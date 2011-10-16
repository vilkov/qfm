#ifndef FILESYSTEMBASEITEM_H_
#define FILESYSTEMBASEITEM_H_

#include "../../../info/filesysteminfo.h"
#include "../../../model/items/filesystemitem.h"


FILE_SYSTEM_NS_BEGIN

class FileSystemBaseItem : public FileSystemItem, public Info
{
public:
	FileSystemBaseItem(const Info &info, const Node::Holder &node, Item *parent = 0);

	void update(const Info &info) { Info::operator=(info); }

	/* IFileInfo */
	virtual QString fileName() const;

	virtual bool isRootItem() const = 0;
	virtual bool isListItem() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMBASEITEM_H_ */
