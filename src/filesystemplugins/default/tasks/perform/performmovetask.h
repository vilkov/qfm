#ifndef PERFORMMOVETASK_H_
#define PERFORMMOVETASK_H_

#include "performcopytask.h"


FILE_SYSTEM_NS_BEGIN

class PerformMoveTask : public PerformCopyTask
{
	Q_DECLARE_TR_FUNCTIONS(PerformMoveTask)

public:
	PerformMoveTask(TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot);

protected:
	virtual void copyFile(const IFileContainer *destination, const IFileContainer *source, InfoItem *entry, volatile bool &tryAgain, const volatile Flags &aborted);

#ifdef Q_OS_UNIX
private:
	QByteArray m_dest;
	QByteArray m_source;
#endif
};

FILE_SYSTEM_NS_END

#endif /* PERFORMMOVETASK_H_ */
