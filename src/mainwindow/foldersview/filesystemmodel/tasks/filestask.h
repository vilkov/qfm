#ifndef FILESTASK_H_
#define FILESTASK_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include <QtCore/QMutex>
#include "../filesysteminfo.h"
#include "../items/filesystemtree.h"
#include "../events/filesystemmodelevents.h"
#include "../../../../tools/taskspool/task.h"


class FilesTask : public TasksPool::Task
{
public:
	struct Params : public QSharedData, public MemoryManagerTag
	{
		QObject *receiver;
		FileSystemTree *fileSystemTree;

	private:
		friend class FilesTask;
	    friend class DeleteHandler;
		QMutex m_mutex;
	};
	typedef QExplicitlySharedDataPointer<Params> ParamsPointer;

	struct EventParams : public FileSystemModelEvent::Params
	{
		FileSystemTree *fileSystemTree;
	};

public:
	/*
	 * This class and it's subclasses should be created only
	 * in the same thread as "receiver" because of DeleteHandler!
	 *
	 */
	FilesTask(Params *parameters);
	virtual ~FilesTask();

	static FileSystemInfo info(const QFileInfo &fileInfo);

protected:
	/* Should be checked as well as "stopedFlag" in "run" function! */
	inline volatile bool isReceiverDead() const { return m_params->receiver == 0; }

	Params *parameters() const { return m_params.data(); }

    FileSystemInfo getInfo(const QFileInfo &fileInfo) const;
#ifndef Q_OS_WIN
    static QFile::Permissions translatePermissions(const QFileInfo &fileInfo, uint userId, uint groupId);
#endif

private:
    friend class DeleteHandler;
    class DeleteHandler : public QObject
    {
    public:
    	DeleteHandler(FilesTask *task, QObject *parent) :
    		QObject(parent),
    		m_task(task),
    		m_params(m_task->m_params)
    	{}
    	virtual ~DeleteHandler()
    	{
    		QMutexLocker locker(&m_params->m_mutex);

    		if (m_task != 0)
				m_task->m_params->receiver = m_task->m_handler = 0;
    	}

    private:
    	friend class FilesTask;
    	FilesTask *m_task;
    	ParamsPointer m_params;
    };

private:
    ParamsPointer m_params;
    DeleteHandler *m_handler;
#ifndef Q_OS_WIN
    uint m_userId;
    uint m_groupId;
#endif
};

#endif /* FILESTASK_H_ */
