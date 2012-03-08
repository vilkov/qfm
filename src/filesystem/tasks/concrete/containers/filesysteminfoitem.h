#ifndef FILESYSTEMINFOITEM_H_
#define FILESYSTEMINFOITEM_H_

#include <QtCore/QList>
#include "../../../info/filesysteminfo.h"


FILE_SYSTEM_NS_BEGIN

class InfoItem : public Info
{
public:
	typedef InfoItem          * value_type;
	typedef QList<value_type>   List;
	typedef List::size_type     size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	explicit InfoItem(const QString &filePath);

	bool isRemoved() const { return m_removed; }
	virtual Info::size_type totalSize() const;

protected:
	friend class PerformRemoveBaseTask;
	void setRemoved(bool value) { m_removed = value; }

private:
	bool m_removed;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINFOITEM_H_ */
