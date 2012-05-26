#ifndef ARCUNPACKACTION_H_
#define ARCUNPACKACTION_H_

#include <QtCore/QCoreApplication>
#include "../libarchive_ns.h"
#include "../../actions/arcaction.h"


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

class UnPackAction : public Action
{
	Q_DECLARE_TR_FUNCTIONS(UnPackAction)

public:
	UnPackAction();

	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const;
};

LIBARCHIVE_ARC_PLUGIN_NS_END

#endif /* ARCUNPACKACTION_H_ */
