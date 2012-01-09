#ifndef ARCREADARCHIVETASK_H_
#define ARCREADARCHIVETASK_H_

#include "../archive/arcarchive.h"
#include "../../../filesystem/tasks/filesystembasetask.h"


ARC_PLUGIN_NS_BEGIN

class ReadArchiveTask : public BaseTask
{
public:
	class Event : public BaseTask::Event
	{
	public:
		enum Type
		{
			ScanComplete = User
		};

	public:
		Event(Type type) :
			BaseTask::Event(static_cast<BaseTask::Event::Type>(type)),
			canceled(false)
		{}

		bool canceled;
		Archive::Contents contents;
	};

public:
	ReadArchiveTask(const QString &fileName, TasksNode *receiver);

protected:
	virtual void run(const volatile bool &aborted);

private:
	QString m_fileName;
};

ARC_PLUGIN_NS_END

#endif /* ARCREADARCHIVETASK_H_ */
