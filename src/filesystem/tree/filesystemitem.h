#ifndef FILESYSTEMITEM_H_
#define FILESYSTEMITEM_H_

#include <QtCore/QList>
#include "../../tools/memory/memory_manager.h"


/*
 *  Base class provides "Compositor" design pattern.
 */
class FileSystemItem : public MemoryManagerTag
{
	Q_DISABLE_COPY(FileSystemItem)

public:
    typedef QList<FileSystemItem*> value_type;
	typedef value_type::size_type  size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	FileSystemItem(FileSystemItem *parent = 0) :
		m_parent(parent)
	{}
	virtual ~FileSystemItem()
	{}

	virtual FileSystemItem *parent() const { return m_parent; }
	virtual FileSystemItem *child(size_type index) const { return 0; }
	virtual size_type size() const { return 0; }
	virtual size_type indexOf(FileSystemItem *item) const { return InvalidIndex; }

protected:
	void setParent(FileSystemItem *parent) { m_parent = parent; }

private:
	FileSystemItem *m_parent;
};

#endif /* FILESYSTEMITEM_H_ */
