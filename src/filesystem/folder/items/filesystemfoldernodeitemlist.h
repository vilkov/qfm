#ifndef FILESYSTEMFOLDERNODEITEMLIST_H_
#define FILESYSTEMFOLDERNODEITEMLIST_H_

#include <QtCore/QList>
#include "filesystemfoldernodeitem.h"


FILE_SYSTEM_NS_BEGIN

class FolderNodeItemList : public FolderNodeItem
{
public:
	typedef FolderNodeItem*      Value;
	typedef QList<Value>         ValueList;
	typedef ValueList::size_type size_type;

public:
	explicit FolderNodeItemList(const Info &info) :
		FolderNodeItem(info)
	{}

	virtual QVariant data(qint32 column, qint32 role) const { return QVariant(); }
	virtual bool isRootItem() const { return false; }
	virtual bool isListItem() const { return true; }

	Value &operator[](size_type index) { return m_items[index]; }
	const Value &operator[](size_type index) const { return m_items[index]; }
	const Value &at(size_type index) const { return m_items.at(index); }
	const Value &last() const { return m_items.last(); }
	Value &last() { return m_items.last(); }

	size_type size() const { return m_items.size(); }
	size_type indexOf(Value item) const { return m_items.indexOf(item); }

	void add(Value item) { m_items.push_back(item); }
	void remove(size_type index) { m_items.removeAt(index); }

private:
	ValueList m_items;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEITEMLIST_H_ */
