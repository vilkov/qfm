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
	virtual bool isListItem() const;

	void lock(const QString &reason, quint64 totalSize);
	void lock(const QString &reason);
	void unlock();

public:
	const QVariant &totalSize() const { return m_totalSize; }
	void setTotalSize(quint64 value) { m_totalSize = value; }
	void clearTotalSize() { m_totalSize.clear(); }

	const QVariant &doneSize() const { return m_doneSize; }
	void setDoneSize(quint64 value) { m_doneSize = value; }

	const QVariant &timeElapsed() const { return m_timeElapsed; }
	void setTimeElapsed(quint64 value) { m_timeElapsed = value; }

	bool isInProgress() const { return !m_doneSize.isNull() && m_totalSize.toULongLong() > 0; }
	bool isCompleted() const { return m_doneSize == m_totalSize; }
	qint32 progress() const { return (m_doneSize.toULongLong() * 100) / m_totalSize.toULongLong(); }

private:
	QVariant m_totalSize;
	QVariant m_doneSize;
	QVariant m_timeElapsed;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMENTRYITEM_H_ */
