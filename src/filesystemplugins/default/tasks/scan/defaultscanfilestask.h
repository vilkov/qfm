#ifndef DEFAULTSCANFILESTASK_H_
#define DEFAULTSCANFILESTASK_H_

#include "../defaultfilesbasetask.h"
#include "../defaultfilesextendedbasetask.h"
#include "../../events/defaultmodelevent.h"


DEFAULT_PLUGIN_NS_BEGIN

class DefaultScanFilesTask : public FilesBaseTask
{
public:
	DefaultScanFilesTask(ModelEvent::Type type, TasksNode *receiver, const Snapshot &snapshot);

protected:
	virtual void run(const volatile Flags &aborted);

private:
	ModelEvent::Type m_type;
	Snapshot m_snapshot;
};


class DefaultScanFilesExtendedTask : public FilesExtendedBaseTask
{
public:
	DefaultScanFilesExtendedTask(ModelEvent::Type type, TasksNode *receiver, ICopyControl::Holder &destination, const Snapshot &snapshot);

protected:
	virtual void run(const volatile Flags &aborted);

private:
	ModelEvent::Type m_type;
	Snapshot m_snapshot;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTSCANFILESTASK_H_ */
