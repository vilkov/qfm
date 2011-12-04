#ifndef SCANFILESTASK_H_
#define SCANFILESTASK_H_

#include <QtCore/QStringList>
#include "../../events/filesystemmodelevent.h"
#include "../../../../tasks/scan/scanfilesbasetask.h"
#include "../../../../tasks/filesystemtasksnode.h"
#include "../../../../../tools/pointers/pscopedpointer.h"


FILE_SYSTEM_NS_BEGIN

class ScanFilesTask : public ScanFilesBaseTask
{
public:
	class Event : public ScanFilesBaseTask::Event
	{
	public:
		Event(ModelEvent::Type type, BaseTask *task, bool canceled, PScopedPointer<InfoListItem> &entries) :
			ScanFilesBaseTask::Event(static_cast<Type>(type)),
			task(task),
			canceled(canceled),
			entries(entries.take())
		{}

		BaseTask *task;
		bool canceled;
		PScopedPointer<InfoListItem> entries;
	};

public:
	ScanFilesTask(TasksNode *receiver, const Info &root, const TasksNode::TasksItemList &files);

	virtual void run(const volatile bool &aborted);

protected:
	const PScopedPointer<InfoListItem> &subnode() const { return m_subnode; }
	PScopedPointer<InfoListItem> &subnode() { return m_subnode; }

private:
	Info m_root;
	TasksNode::TasksItemList m_files;
	PScopedPointer<InfoListItem> m_subnode;
};

FILE_SYSTEM_NS_END

#endif /* SCANFILESTASK_H_ */
