#ifndef DESTCONTROLABLETASK_H_
#define DESTCONTROLABLETASK_H_

#include <QtCore/QObject>
#include "basetask.h"
#include "../../../tools/threads/pmutex.h"


FILE_SYSTEM_NS_BEGIN

class DestControlableTask : public BaseTask
{
public:
	/*
	 * This class and it's subclasses should be created only
	 * in the same thread as "destination.object" because of DeleteHandler!
	 *
	 */
	DestControlableTask(QObject *receiver);
	virtual ~DestControlableTask();

protected:
    inline QObject *receiver() const { return m_receiver; }
	inline const volatile bool &isControllerDead() const { return m_controllerDead; }

private:
	struct MutexHolder : public QSharedData
	{
		PMutex mutex;
	};
	typedef QExplicitlySharedDataPointer<MutexHolder> MutexHolderPointer;
	class DeleteHandler;

private:
    MutexHolderPointer m_mutexHolder;
    QObject *m_receiver;
    DeleteHandler *m_handler;
	volatile bool m_controllerDead;
};

FILE_SYSTEM_NS_END

#endif /* DESTCONTROLABLETASK_H_ */
