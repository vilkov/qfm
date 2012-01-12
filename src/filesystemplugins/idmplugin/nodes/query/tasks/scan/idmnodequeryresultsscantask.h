#ifndef IDMNODEQUERYRESULTSSCANTASK_H_
#define IDMNODEQUERYRESULTSSCANTASK_H_

#include <QtCore/QMap>
#include "../../events/idmqueryresultsmodelevents.h"
#include "../../../../../../filesystem/tasks/scan/scanfilesbasetask.h"


IDM_PLUGIN_NS_BEGIN

class ScanFilesTask : public ScanFilesBaseTask
{
public:
	class Event : public ScanFilesBaseTask::Event
	{
	public:
		Event(BaseTask *task, ModelEvent::Type type) :
			ScanFilesBaseTask::Event(task, static_cast<ScanFilesBaseTask::Event::Type>(type))
		{}

		ScanedFiles files;
	};

public:
	ScanFilesTask(TasksNode *receiver, const TasksNode::TasksItemList &files);

	virtual void run(const volatile Flags &aborted);

protected:
	ScanedFiles scan(const volatile Flags &aborted) const;

private:
	TasksNode::TasksItemList m_files;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEQUERYRESULTSSCANTASK_H_ */
