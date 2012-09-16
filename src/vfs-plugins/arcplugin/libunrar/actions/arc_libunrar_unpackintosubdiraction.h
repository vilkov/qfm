#ifndef ARC_LIBUNRAR_UNPACKINTOSUBDIRACTION_H_
#define ARC_LIBUNRAR_UNPACKINTOSUBDIRACTION_H_

#include <QtCore/QCoreApplication>
#include "../arc_libunrar_ns.h"
#include "../../actions/arc_action.h"


ARC_PLUGIN_LIBUNRAR_NS_BEGIN

class UnPackIntoSubdirAction : public Action
{
	Q_DECLARE_TR_FUNCTIONS(UnPackIntoSubdirAction)

public:
	UnPackIntoSubdirAction();

	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const;
};

ARC_PLUGIN_LIBUNRAR_NS_END

#endif /* ARC_LIBUNRAR_UNPACKINTOSUBDIRACTION_H_ */
