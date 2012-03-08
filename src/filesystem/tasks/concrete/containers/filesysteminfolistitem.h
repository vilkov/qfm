#ifndef FILESYSTEMINFOLISTITEM_H_
#define FILESYSTEMINFOLISTITEM_H_

#include "filesysteminfoitem.h"
#include "../../../info/filesystemfilecontainer.h"


FILE_SYSTEM_NS_BEGIN

class InfoListItem : public InfoItem
{
public:
	explicit InfoListItem(const QString &filePath);
	virtual ~InfoListItem();

	/* InfoItem */
	virtual Info::size_type totalSize() const;

	const FileContainer &container() const { return m_container; }

	value_type &operator[](size_type index) { return m_items[index]; }
	const value_type &operator[](size_type index) const { return m_items[index]; }

	bool isEmpty() const { return m_items.isEmpty(); }
	const value_type &at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(value_type item) const { return m_items.indexOf(item); }

protected:
	friend class ScanFilesBaseTask;

	const value_type &last() const { return m_items.last(); }
	value_type &last() { return m_items.last(); }

	void add(value_type item) { m_items.push_back(item); m_totalSize += item->totalSize(); }
	void remove(size_type index) { value_type item = m_items.takeAt(index); m_totalSize -= item->totalSize(); delete item; }

private:
	List m_items;
	FileContainer m_container;
	Info::size_type m_totalSize;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINFOLISTITEM_H_ */
