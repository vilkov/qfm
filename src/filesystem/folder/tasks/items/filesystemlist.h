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
		FileSystemItem(info),
		m_data(new Data())
	{}
#ifndef Q_OS_WIN
	explicit FileSystemList(const Info &info, uint userId, uint groupId) :
		FileSystemItem(info),
		m_data(new Data())
	{}
#endif

	virtual bool isList() const { return true; }

	value_type &operator[](size_type index) { return m_data->items[index]; }
	const value_type &operator[](size_type index) const { return m_data->items[index]; }

	const value_type &at(size_type index) const { return m_data->items.at(index); }
	size_type size() const { return m_data->items.size(); }
	size_type indexOf(value_type item) const { return m_data->items.indexOf(item); }

	const value_type &last() const { return m_data->items.last(); }
	value_type &last() { return m_data->items.last(); }

	void add(value_type item) { return m_data->items.push_back(item); }
	void remove(size_type index) { return m_data->items.removeAt(index); }

private:
	struct Data : QSharedData
	{
		~Data()
		{
			qDeleteAll(items);
		}

		List items;
	};

private:
	QExplicitlySharedDataPointer<Data> m_data;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMLIST_H_ */
