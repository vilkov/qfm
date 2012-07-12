#ifndef LIBARCHIVEUNPACKINTOSUBDIRACTION_H_
#define LIBARCHIVEUNPACKINTOSUBDIRACTION_H_

#include <QtCore/QCoreApplication>
#include "../libarchive_ns.h"
#include "../../actions/arcaction.h"


LIBARCHIVE_ARC_PLUGIN_NS_BEGIN

class UnPackIntoSubdirAction : public Action
{
	Q_DECLARE_TR_FUNCTIONS(UnPackIntoSubdirAction)

public:
	UnPackIntoSubdirAction();

	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const;
};

LIBARCHIVE_ARC_PLUGIN_NS_END

#endif /* LIBARCHIVEUNPACKINTOSUBDIRACTION_H_ */
