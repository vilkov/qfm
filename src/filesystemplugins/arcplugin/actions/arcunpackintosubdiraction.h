#ifndef ARCUNPACKINTOSUBDIRACTION_H_
#define ARCUNPACKINTOSUBDIRACTION_H_

#include <QtCore/QCoreApplication>
#include "arcaction.h"


ARC_PLUGIN_NS_BEGIN

class UnPackIntoSubdirAction : public Action
{
	Q_DECLARE_TR_FUNCTIONS(UnPackIntoSubdirAction)

public:
	UnPackIntoSubdirAction();

	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const;
};

ARC_PLUGIN_NS_END

#endif /* ARCUNPACKINTOSUBDIRACTION_H_ */
