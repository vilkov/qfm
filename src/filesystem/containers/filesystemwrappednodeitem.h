#ifndef FILESYSTEMWRAPPEDNODEITEM_H_
#define FILESYSTEMWRAPPEDNODEITEM_H_

#include <QtCore/QList>
#include "../interfaces/filesystemifilecontainer.h"


FILE_SYSTEM_NS_BEGIN

class WrappedNodeItem
{
	Q_DISABLE_COPY(WrappedNodeItem)

public:
	typedef PScopedPointer<WrappedNodeItem> Holder;
	typedef QList<WrappedNodeItem *>        Container;
	typedef Container::iterator             iterator;
	typedef Container::const_iterator       const_iterator;
	typedef Container::size_type            size_type;

public:
	WrappedNodeItem();
	WrappedNodeItem(const IFileContainer *container, IFileInfo::Holder &info, WrappedNodeItem *parent);
	virtual ~WrappedNodeItem();

	bool isValid() const { return m_container != NULL; }

	bool isEmpty() const { return m_items.isEmpty(); }
	size_type size() const { return m_items.size(); }

	const_iterator begin() const { return m_items.begin(); }
	iterator begin() { return m_items.begin(); }

	const_iterator end() const { return m_items.end(); }
	iterator end() { return m_items.end(); }

	void append(WrappedNodeItem *item)
	{
		m_items.append(item);
		incTotalSize(item->totalSize());
	}

	iterator erase(iterator i)
	{
		Holder item(*i);
		decTotalSize(item->totalSize());
		return m_items.erase(i);
	}

	const IFileContainer::Holder &thisContainer() const { return m_thisContainer; }
	IFileContainer::Holder &thisContainer() { return m_thisContainer; }

	const IFileContainer *container() const { return m_container; }
	IFileInfo::size_type totalSize() const { return m_totalSize; }

	bool isRemoved() const { return m_removed; }
	void setRemoved(bool value) { m_removed = value; }

	const IFileInfo::Holder &info() const { return m_info; }
	IFileInfo::Holder &info() { return m_info; }

protected:
	WrappedNodeItem(const IFileContainer *container, WrappedNodeItem *parent);
	WrappedNodeItem(const IFileContainer *container, IFileInfo::size_type totalSize, WrappedNodeItem *parent);

protected:
	Container &items() { return m_items; }

	void incTotalSize(IFileInfo::size_type size)
	{
		WrappedNodeItem *parent = this;

		do
			parent->m_totalSize += size;
		while (parent = parent->m_parent);
	}

	void decTotalSize(IFileInfo::size_type size)
	{
		WrappedNodeItem *parent = this;

		do
			parent->m_totalSize -= size;
		while (parent = parent->m_parent);
	}

private:
	IFileContainer::Holder m_thisContainer;
	const IFileContainer *m_container;
	IFileInfo::size_type m_totalSize;
	IFileInfo::Holder m_info;
	WrappedNodeItem *m_parent;
	Container m_items;
	bool m_removed;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMWRAPPEDNODEITEM_H_ */
