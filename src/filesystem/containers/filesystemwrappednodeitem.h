#ifndef FILESYSTEMWRAPPEDNODEITEM_H_
#define FILESYSTEMWRAPPEDNODEITEM_H_

#include <QtCore/QMap>
#include "../interfaces/filesystemifilecontainer.h"


FILE_SYSTEM_NS_BEGIN

class WrappedNodeItem
{
	Q_DISABLE_COPY(WrappedNodeItem)

public:
	typedef PScopedPointer<WrappedNodeItem>  Holder;
	typedef QMap<QString, WrappedNodeItem *> Container;
	typedef Container::iterator              iterator;
	typedef Container::const_iterator        const_iterator;
	typedef Container::size_type             size_type;

public:
	WrappedNodeItem();
	WrappedNodeItem(const IFileContainer *container, IFileInfo::Holder &info, WrappedNodeItem *parent);
	~WrappedNodeItem();

	bool isValid() const { return m_container != NULL; }

	bool isEmpty() const { return m_items.isEmpty(); }
	size_type size() const { return m_items.size(); }

	const_iterator begin() const { return m_items.begin(); }
	iterator begin() { return m_items.begin(); }

	const_iterator end() const { return m_items.end(); }
	iterator end() { return m_items.end(); }

	WrappedNodeItem *find(const QString &fileName) const { return m_items.value(fileName); }

	void insert(const QString &fileName, WrappedNodeItem *item)
	{
		WrappedNodeItem *&value = m_items[fileName];
		delete value;
		incTotalSize((value = item)->totalSize());
	}

	iterator erase(iterator i)
	{
		decTotalSize((*i)->totalSize());
		delete (*i);
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

private:
	void incTotalSize(IFileInfo::size_type size)
	{
		WrappedNodeItem *parent = this;

		while (parent)
		{
			parent->m_totalSize += size;
			parent = parent->m_parent;
		}
	}

	void decTotalSize(IFileInfo::size_type size)
	{
		WrappedNodeItem *parent = this;

		while (parent)
		{
			parent->m_totalSize -= size;
			parent = parent->m_parent;
		}
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
