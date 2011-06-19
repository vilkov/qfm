#ifndef FILESYSTEMLIST_H_
#define FILESYSTEMLIST_H_

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include "filesystemitem.h"


FILE_SYSTEM_NS_BEGIN

class FileSystemList : public FileSystemItem
{
public:
	explicit FileSystemList(const Info &info) :
		FileSystemItem(info)
	{}
#ifndef Q_OS_WIN
	explicit FileSystemList(const Info &info, uint userId, uint groupId) :
		FileSystemItem(info)
	{}
#endif
	virtual ~FileSystemList()
	{
		qDeleteAll(m_items);
	}

	virtual bool isList() const { return true; }

	value_type &operator[](size_type index) { return m_items[index]; }
	const value_type &operator[](size_type index) const { return m_items[index]; }

	const value_type &at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(value_type item) const { return m_items.indexOf(item); }

	const value_type &last() const { return m_items.last(); }
	value_type &last() { return m_items.last(); }

	void add(value_type item) { m_items.push_back(item); }
	void remove(size_type index) { delete m_items.takeAt(index); }

private:
	List m_items;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMLIST_H_ */
