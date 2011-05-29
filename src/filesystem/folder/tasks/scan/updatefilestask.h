#ifndef UPDATEFILESTASK_H_
#define UPDATEFILESTASK_H_

#include "../controlabletask.h"
#ifndef Q_OS_WIN
#	include "../taskpermissionscache.h"
#endif
#include "../../containers/filesystemupdateslist.h"


FILE_SYSTEM_NS_BEGIN

class UpdateFilesTask : public ControlableTask
{
public:
	typedef ControlableTask parent_class;

public:
	struct Params : public parent_class::Params
	{
		Params(Node *node, const UpdatesList &list) :
			node(node),
			list(list)
		{}

		Node *node;
		UpdatesList list;
	};
	typedef QScopedPointer<Params> ParamsPointer;
	typedef ModelEvents::UpdateFilesEvent Event;

public:
	UpdateFilesTask(ParamsPointer &params);

	virtual void run(const volatile bool &stopedFlag);

protected:
	Params *parameters() const { return static_cast<Params*>(parent_class::parameters()); }

#ifndef Q_OS_WIN
private:
	TaskPermissionsCache m_permissions;
#endif
};

FILE_SYSTEM_NS_END

#endif /* UPDATEFILESTASK_H_ */
