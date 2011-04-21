#ifndef CONTROLABLETASK_H_
#define CONTROLABLETASK_H_

#include <QtCore/QSharedData>
#include "../items/filesystemtree.h"
#include "../events/filesystemmodelevents.h"
#include "../../../../tools/taskspool/task.h"


class ControlableTask : public TasksPool::Task
{
public:
	struct Params : public QSharedData, public MemoryManagerTag
	{
		struct Listener
		{
			Listener() :
				object(0),
				fileSystemTree(0)
			{}
			Listener(QObject *object, FileSystemTree *fileSystemTree) :
				object(object),
				fileSystemTree(fileSystemTree)
			{}

			QObject *object;
			FileSystemTree *fileSystemTree;
		};
		struct Snapshot
		{
			Snapshot() :
				object(0),
				fileSystemTree(0),
				entry(0)
			{}
			Snapshot(QObject *object, FileSystemTree *fileSystemTree, FileSystemEntry *entry) :
				object(object),
				fileSystemTree(fileSystemTree),
				entry(entry)
			{}

			QObject *object;
			FileSystemTree *fileSystemTree;
			FileSystemEntry *entry;
		};
	};
	struct EventParams : public FileSystemModelEvent::EventParams
	{
		struct Snapshot
		{
			Snapshot() :
				fileSystemTree(0),
				entry(0)
			{}
			Snapshot(FileSystemTree *fileSystemTree, FileSystemEntry *entry) :
				fileSystemTree(fileSystemTree),
				entry(entry)
			{}
			Snapshot(const Params::Snapshot &listener) :
				fileSystemTree(listener.fileSystemTree),
				entry(listener.entry)
			{}

			Snapshot &operator=(const Params::Snapshot &other)
			{
				fileSystemTree = other.fileSystemTree;
				entry = other.entry;
				return *this;
			}

			FileSystemTree *fileSystemTree;
			FileSystemEntry *entry;
		};
	};

public:
	ControlableTask(Params *parameters);

protected:
	inline const volatile bool &isControllerDead() const { return m_controllerDead; }

protected:
	typedef QExplicitlySharedDataPointer<Params> ParamsPointer;
	inline Params *parameters() const { return m_params.data(); }
	const ParamsPointer &params() const { return m_params; }

private:
    ParamsPointer m_params;
	volatile bool m_controllerDead;
};

#endif /* CONTROLABLETASK_H_ */
