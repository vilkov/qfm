#ifndef ARCUNPACKHEREACTION_H_
#define ARCUNPACKHEREACTION_H_

#include <QtCore/QCoreApplication>
#include "../libarchive_ns.h"
#include "../../actions/arcaction.h"


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

class UnPackHereAction : public Action
{
	Q_DECLARE_TR_FUNCTIONS(UnPackHereAction)

public:
	UnPackHereAction();

	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const;
};

LIBARCHIVE_ARC_PLUGIN_NS_END

#endif /* ARCUNPACKHEREACTION_H_ */
