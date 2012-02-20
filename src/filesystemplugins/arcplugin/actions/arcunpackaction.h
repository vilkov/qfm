#ifndef ARCUNPACKACTION_H_
#define ARCUNPACKACTION_H_

#include <QtCore/QCoreApplication>
#include "arcaction.h"


ARC_PLUGIN_NS_BEGIN

class UnPackAction : public Action
{
	Q_DECLARE_TR_FUNCTIONS(UnPackAction)

public:
	UnPackAction();

	virtual PerformActionTask *process(TasksNode *receiver, const FilesList &files) const;
};

ARC_PLUGIN_NS_END

#endif /* ARCUNPACKACTION_H_ */
