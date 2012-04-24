#ifndef FILESYSTEMWRAPPEDNODEITEM_H_
#define FILESYSTEMWRAPPEDNODEITEM_H_

#include <QtCore/QList>
#include "../interfaces/filesystemifileinfo.h"
#include "../interfaces/filesystemifilecontainer.h"


FILE_SYSTEM_NS_BEGIN

class WrappedNodeItem
{
public:
	typedef PScopedPointer<WrappedNodeItem> Holder;
	typedef QList<WrappedNodeItem *>        List;
	typedef List::size_type                 size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	WrappedNodeItem();
	WrappedNodeItem(const IFileContainer *container, IFileInfo *item);
	~WrappedNodeItem();

	bool isValid() const { return m_container != NULL; }

	size_type size() const { return m_items.size(); }
	WrappedNodeItem *at(size_type index) const { return m_items.at(index); }

	const IFileContainer *container() const { return m_container; }
	IFileInfo::size_type totalSize() const { return m_totalSize; }
	bool isRemoved() const { return m_removed; }

	const IFileInfo::Holder &item() const { return m_item; }
	IFileInfo::Holder &item() { return m_item; }

private:
	const IFileContainer *m_container;
	IFileInfo::size_type m_totalSize;
	IFileInfo::Holder m_item;
	bool m_removed;
	List m_items;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMWRAPPEDNODEITEM_H_ */
