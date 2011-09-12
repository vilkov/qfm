#ifndef FILESYSTEMITEM_H_
#define FILESYSTEMITEM_H_

#include <QtCore/QList>
#include "../info/filesysteminfo.h"


FILE_SYSTEM_NS_BEGIN

class FileSystemItem : public Info
{
public:
	typedef FileSystemItem    * value_type;
	typedef QList<value_type>   List;
	typedef List::size_type     size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	FileSystemItem(const Info &info) :
		Info(info),
		m_shouldRemove(true)
	{}

	virtual bool isList() const = 0;
	virtual qint64 totalSize() const = 0;

	bool shouldRemove() const { return m_shouldRemove; }
	void setShouldRemove(bool value) { m_shouldRemove = value; }

private:
	bool m_shouldRemove;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMITEM_H_ */
