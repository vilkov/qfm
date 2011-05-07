#ifndef UPDATEFILESTASK_H_
#define UPDATEFILESTASK_H_

#include "filestask.h"
#include "../controlabletask.h"
#include "../../filesystemchangeslist.h"


FILE_SYSTEM_NS_BEGIN

class UpdateFilesTask : public TemplateFilesTask<ControlableTask>
{
public:
	typedef TemplateFilesTask<ControlableTask> parent_class;

public:
	struct Params : public parent_class::Params
	{
		Node *node;
		ChangesList list;
	};
	typedef ModelEvents::UpdateFilesEvent Event;

public:
	UpdateFilesTask(Params *params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }

private:
    ChangesList::size_type indexOf(const QString &directoryPath, const ChangesList &list) const;
};

FILE_SYSTEM_NS_END

#endif /* UPDATEFILESTASK_H_ */
