#ifndef FILESYSTEMTASKNODEITEM_H_
#define FILESYSTEMTASKNODEITEM_H_

#include <QtGui/QIcon>
#include "../../model/items/filesystemitem.h"


FILE_SYSTEM_NS_BEGIN

class TaskNodeItem : public FileSystemItem
{
public:
	TaskNodeItem(Base *parent);

	bool isLocked() const { return m_locked; }
	const QString &lockReason() const { return m_reason; }
	const QIcon &lockIcon() const { return m_icon; }

	void lock(const QString &reason);
	void cancel(const QString &reason);
    void unlock() { m_locked = false; m_reason.clear(); m_icon = QIcon(); }

	bool isInProgress() const { return !m_done.isNull(); }
	bool isCompleted() const { return m_done == m_total; }

	quint64 done() const { return m_done.toULongLong(); }
	quint64 total() const { return m_total; }
	quint64 timeElapsed() const { return m_timeElapsed; }

	qint32 progress() const { return (m_done.toULongLong() * 100) / m_total; }

protected:
	void start(quint64 total) { m_done = quint64(1); m_total = total < 1 ? 1 : total; }
	void progress(quint64 value, quint64 timeElapsed) { m_done = value; m_timeElapsed = timeElapsed; }
	void stop() { m_done.clear(); m_total = 0; m_timeElapsed = 0; }

private:
	bool m_locked;
	QString m_reason;
	QIcon m_icon;
	QVariant m_done;
	quint64 m_total;
	quint64 m_timeElapsed;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKNODEITEM_H_ */
