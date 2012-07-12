#ifndef LIBUNRARUNPACKINTOSUBDIRACTION_H_
#define LIBUNRARUNPACKINTOSUBDIRACTION_H_

#include <QtCore/QCoreApplication>
#include "../libunrar_ns.h"
#include "../../actions/arcaction.h"


LIBUNRAR_ARC_PLUGIN_NS_BEGIN

class UnPackIntoSubdirAction : public Action
{
	Q_DECLARE_TR_FUNCTIONS(UnPackIntoSubdirAction)

public:
	UnPackIntoSubdirAction();

	virtual PerformActionTask *process(TasksNode *receiver, const IFileContainer *container, const FilesList &files) const;
};

LIBUNRAR_ARC_PLUGIN_NS_END

#endif /* LIBUNRARUNPACKINTOSUBDIRACTION_H_ */
