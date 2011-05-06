#ifndef FILESYSTEMFOLDERNODEENTRY_H_
#define FILESYSTEMFOLDERNODEENTRY_H_

#include "filesystemfoldernodeitem.h"
#include "../filesystemchangeslist.h"


FILE_SYSTEM_NS_BEGIN

class FolderNodeEntry : public FolderNodeItem
{
	Q_DISABLE_COPY(FolderNodeEntry)

public:
	FolderNodeEntry(const Info &info);

	/* FolderNodeEntry */
	virtual QVariant data(qint32 column, qint32 role) const;
	virtual bool isRoot() const { return false; }

	/* IFileInfo */
	virtual bool exists() const { return m_info.exists(); }
	virtual QString fileName() const { return m_info.fileName(); }
	virtual QString absolutePath() const { return m_info.absolutePath(); }
	virtual QString absoluteFilePath() const { return m_info.absoluteFilePath(); }
	virtual QDateTime lastModified() const { return m_info.lastModified(); }

	virtual void refresh() { m_info.refresh(); }

public:
	bool isDir() const { return m_info.isDir(); }
	qint64 size() const { return m_info.size(); }

	void update(const Info &info) { m_info = info; }

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
	Info m_info;
	bool m_locked;
	QString m_lockReason;
	QVariant m_totalSize;
	QVariant m_doneSize;
	QVariant m_timeElapsed;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERNODEENTRY_H_ */
