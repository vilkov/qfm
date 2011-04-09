#ifndef FILESTASK_H_
#define FILESTASK_H_

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QFileInfo>
#include "../filesysteminfo.h"
#include "../items/filesystemtree.h"
#include "../events/filesystemmodelevents.h"
#include "../../../../tools/taskspool/task.h"


class FilesTask : public TasksPool::Task
{
public:
	struct EventParams : public FileSystemModelEvent::Params
	{
		FileSystemTree *fileSystemTree;
	};

public:
	/* This class and it's subclasses should be created only in the same thread as "receiver" */
	FilesTask(FileSystemTree *tree, QObject *receiver);
	virtual ~FilesTask();

	static FileSystemInfo info(const QFileInfo &fileInfo);

protected:
    FileSystemTree *tree() const { return m_tree; }
	QObject *receiver() const { return m_receiver; }
	inline volatile bool isReceiverDead() const { return m_receiver == 0; }

    FileSystemInfo getInfo(const QFileInfo &fileInfo) const;
#ifndef Q_OS_WIN
    static QFile::Permissions translatePermissions(const QFileInfo &fileInfo, uint userId, uint groupId);
#endif

private:
    class DeleteHandler : public QObject
    {
    public:
    	DeleteHandler(FilesTask *task, QObject *parent) :
    		QObject(parent),
    		m_task(task),
    		m_isDetached(false)
    	{
    		Q_ASSERT(task != 0);
    	}
    	virtual ~DeleteHandler()
    	{
    		if (m_task != 0) /* TODO: Fix it - race condition! */
				m_task->m_receiver = m_task->m_handler = 0;
    	}

    private:
    	friend class FilesTask;
    	FilesTask *m_task;
    	bool m_isDetached;
    };
    friend class DeleteHandler;

private:
    FileSystemTree *m_tree;
	QObject *m_receiver;
    DeleteHandler *m_handler;
#ifndef Q_OS_WIN
    uint m_userId;
    uint m_groupId;
#endif
};

#endif /* FILESTASK_H_ */
