#ifndef FILESYSTEMINFOLISTITEM_H_
#define FILESYSTEMINFOLISTITEM_H_

#include <QtCore/QList>
#include <QtCore/QSharedData>
#include "filesysteminfoitem.h"


FILE_SYSTEM_NS_BEGIN

class InfoListItem : public InfoItem
{
public:
	explicit InfoListItem(const Info &info);
#ifndef Q_OS_WIN
	explicit InfoListItem(const Info &info, uint userId, uint groupId);
#endif
	virtual ~InfoListItem();

	virtual bool isList() const;
	virtual qint64 totalSize() const;

	value_type &operator[](size_type index) { return m_items[index]; }
	const value_type &operator[](size_type index) const { return m_items[index]; }

	bool isEmpty() const { return m_items.isEmpty(); }
	const value_type &at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(value_type item) const { return m_items.indexOf(item); }

protected:
	friend class ScanFilesBaseTask;

	void incTotalSize(qint64 count) { m_totalSize += count; }
	void decTotalSize(qint64 count) { m_totalSize -= count; }

	const value_type &last() const { return m_items.last(); }
	value_type &last() { return m_items.last(); }

	void add(value_type item) { m_items.push_back(item); }
	void remove(size_type index) { delete m_items.takeAt(index); }

private:
	List m_items;
	qint64 m_totalSize;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINFOLISTITEM_H_ */
