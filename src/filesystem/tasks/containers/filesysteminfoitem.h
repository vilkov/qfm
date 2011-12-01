#ifndef FILESYSTEMINFOITEM_H_
#define FILESYSTEMINFOITEM_H_

#include <QtCore/QList>
#include "../../info/filesysteminfo.h"


FILE_SYSTEM_NS_BEGIN

class InfoItem : public Info
{
public:
	typedef InfoItem          * value_type;
	typedef QList<value_type>   List;
	typedef List::size_type     size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	InfoItem(const Info &info);

	virtual bool isList() const = 0;
	virtual qint64 totalSize() const = 0;

	bool shouldRemove() const { return m_shouldRemove; }

protected:
	friend class PerformRemoveBaseTask;

	void setShouldRemove(bool value) { m_shouldRemove = value; }

private:
	bool m_shouldRemove;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINFOITEM_H_ */
