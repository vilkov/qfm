#ifndef FILESYSTEMENTRYITEM_H_
#define FILESYSTEMENTRYITEM_H_

#include "../base/items/filesystembaseitem.h"


FILE_SYSTEM_NS_BEGIN

class FileSystemEntryItem : public FileSystemBaseItem
{
public:
	explicit FileSystemEntryItem(const Info &info, Node *node = 0, Item *parent = 0);

	void setNode(Node *node) { FileSystemBaseItem::setNode(node); }

	/* ::Tools::Models::Tree::Item */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* FileSystemBaseItem */
	virtual bool isRootItem() const;
	virtual bool isListItem() const;

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

	bool isLocked() const { return m_locked; }
	const QString &lockReason() const { return m_lockReason; }
	void lock(const QString &reason, quint64 totalSize);
	void lock(const QString &reason);
	void unlock();

private:
	bool m_locked;
	QString m_lockReason;
	QVariant m_totalSize;
	QVariant m_doneSize;
	QVariant m_timeElapsed;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMENTRYITEM_H_ */
