#ifndef FILESYSTEMENTRYITEM_H_
#define FILESYSTEMENTRYITEM_H_

#include "../base/items/filesystembaseitem.h"


FILE_SYSTEM_NS_BEGIN

class FileSystemEntryItem : public FileSystemBaseItem
{
public:
	explicit FileSystemEntryItem(const Info &info, Base *parent = 0);
	explicit FileSystemEntryItem(const Info &info, Node *node, Base *parent = 0);

	void setNode(Node *node) { FileSystemBaseItem::setNode(node); }

	/* ::Tools::Models::Tree::Item */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* FileSystemBaseItem */
	virtual bool isRootItem() const;

	void lock(const QString &reason, quint64 totalSize);
	void lock(const QString &reason);
	void unlock();

public:
	const QVariant &totalSize() const { return m_totalSize; }
	void setTotalSize(quint64 value) { m_totalSize = value; }
	void clearTotalSize() { m_totalSize.clear(); }

	void updateProgress(quint64 value, quint64 timeElapsed) { progress(value, timeElapsed); }

private:
	QVariant m_totalSize;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMENTRYITEM_H_ */
