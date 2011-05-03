#ifndef FILESYSTEMENTRY_H_
#define FILESYSTEMENTRY_H_

#include <QDir>
#include <QString>
#include "filesystemitem.h"
#include "../filesystemchangeslist.h"


class FileSystemEntry : public FileSystemItem
{
	Q_DISABLE_COPY(FileSystemEntry)

public:
	FileSystemEntry(const FileSystemInfo &info, FileSystemItem *parent = 0);
	FileSystemEntry(const Change &change, FileSystemItem *parent = 0);

	virtual QVariant data(qint32 column, qint32 role) const;
	virtual void accept(FileSystemModelVisitor *visitor) const;

	const FileSystemInfo &fileInfo() const { return m_info; }
	FileSystemInfo &fileInfo() { return m_info; }

	void update(const FileSystemInfo &info) { m_info = info; }

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
	FileSystemInfo m_info;
};

#endif /* FILESYSTEMENTRY_H_ */
