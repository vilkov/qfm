#ifndef ARCUNPACKHEREACTION_H_
#define ARCUNPACKHEREACTION_H_

#include <QtCore/QCoreApplication>
#include "arcaction.h"


ARC_PLUGIN_NS_BEGIN

class UnPackHereAction : public Action
{
	Q_DECLARE_TR_FUNCTIONS(UnPackHereAction)

public:
	UnPackHereAction();

	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const;
};

ARC_PLUGIN_NS_END

#endif /* ARCUNPACKHEREACTION_H_ */
