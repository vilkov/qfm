#ifndef ARC_LIBARCHIVE_UNPACKINTOSUBDIRACTION_H_
#define ARC_LIBARCHIVE_UNPACKINTOSUBDIRACTION_H_

#include <QtCore/QCoreApplication>
#include "../arc_libarchive_ns.h"
#include "../../actions/arc_action.h"


ARC_PLUGIN_LIBARCHIVE_NS_BEGIN

class UnPackIntoSubdirAction : public Action
{
	Q_DECLARE_TR_FUNCTIONS(UnPackIntoSubdirAction)

public:
	UnPackIntoSubdirAction();

	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const;
};

ARC_PLUGIN_LIBARCHIVE_NS_END

#endif /* ARC_LIBARCHIVE_UNPACKINTOSUBDIRACTION_H_ */
