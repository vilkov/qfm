#ifndef FILESYSTEMFOLDERNODEENTRY_H_
#define FILESYSTEMFOLDERNODEENTRY_H_

#include "filesystemfoldernodeitem.h"
#include "../filesystemchangeslist.h"
#include "../info/filesystemfoldernodeinfo.h"


FILE_SYSTEM_NS_BEGIN

class FolderNodeEntry : public FolderNodeItem
{
public:
	explicit FolderNodeEntry(const Info &info) :
		FolderNodeItem(info),
		m_locked(false)
	{}

	void operator=(const Info &info) { FolderNodeItem::operator=(info); }

	/* FolderNodeEntry */
	virtual QVariant data(qint32 column, qint32 role) const;
	virtual bool isRootItem() const { return false; }

public:
	const QVariant &totalSize() const { return m_totalSize; }
	void setTotalSize(quint64 value) { m_totalSize = value; }
	void clearTotalSize() { m_totalSize.clear(); }

	const QVariant &doneSize() const { return m_doneSize; }
	void setDoneSize(quint64 value) { m_doneSize = value; }

	const QVariant &timeElapsed() const { return m_timeElapsed; }
	void setTimeElapsed(quint64 value) { m_timeElapsed = value; }

	bool isInProgress() const { return !m_doneSize.isNull(); }
	qint32 progress() const { return (m_doneSize.toULongLong() * 100) / m_totalSize.toULongLong(); }

	bool isLocked() const { return m_locked; }
	const QString &lockReason() const { return m_lockReason; }
	void lock(const QString &reason, quint64 totalSize);
	void lock(const QString &reason);
	void unlock();

	static QString humanReadableTime(quint64 msecs);
	static QString humanReadableSize(quint64 size);
	static QString humanReadableShortSize(quint64 size);

private:
	bool m_locked;
	QString m_lockReason;
	QVariant m_totalSize;
	QVariant m_doneSize;
	QVariant m_timeElapsed;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEENTRY_H_ */
