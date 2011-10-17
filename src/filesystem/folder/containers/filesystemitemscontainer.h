#ifndef FILESYSTEMITEMSCONTAINER_H_
#define FILESYSTEMITEMSCONTAINER_H_

#include "../base/items/filesystembaseitem.h"
#include "../../model/filesystemmodelcontainer.h"
#include "../../../tools/containers/hashedlist.h"


FILE_SYSTEM_NS_BEGIN

class ItemsContainer : public FileSystemModelContainer
{
public:
	ItemsContainer();
	virtual ~ItemsContainer();

	virtual size_type size() const;
	virtual Item *at(size_type index) const;
	virtual size_type indexOf(Item *item) const;

private:
	/* Just to avoid access through virtual methods. */
	friend class FolderNode;
	friend class RootNode;

private:
	typedef ::Tools::Containers::HashedList<QString, FileSystemBaseItem *> Container;
	Container m_container;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMITEMSCONTAINER_H_ */
