#ifndef FILESYSTEMTASKNODEITEM_H_
#define FILESYSTEMTASKNODEITEM_H_

#include "../../model/items/filesystemitem.h"
#include "../../../tools/models/macros/declareitemlocker.h"


FILE_SYSTEM_NS_BEGIN

class TaskNodeItem : public FileSystemItem
{
	DECLARE_MODEL_ITEM_LOCKER;

public:
	TaskNodeItem(Base *parent);

	void lock(const QString &reason);
    void unlock() { MODEL_ITEM_LOCKER_UNLOCK; }

	bool isInProgress() const { return !m_done.isNull(); }
	bool isCompleted() const { return m_done == m_total; }

	quint64 done() const { return m_done.toULongLong(); }
	quint64 total() const { return m_total; }
	quint64 timeElapsed() const { return m_timeElapsed; }

	qint32 progress() const { return (m_done.toULongLong() * 100) / m_total; }

protected:
	void start(quint64 total) { m_done = quint64(0); m_total = total; }
	void progress(quint64 value, quint64 timeElapsed) { m_done = value; m_timeElapsed = timeElapsed; }
	void stop() { m_done.clear(); m_total = 0; m_timeElapsed = 0; }

private:
	QVariant m_done;
	quint64 m_total;
	quint64 m_timeElapsed;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMTASKNODEITEM_H_ */
