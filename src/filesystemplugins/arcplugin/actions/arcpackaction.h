#ifndef ARCPACKACTION_H_
#define ARCPACKACTION_H_

#include <QtCore/QCoreApplication>
#include "arcaction.h"


ARC_PLUGIN_NS_BEGIN

class PackAction : public Action
{
	Q_DECLARE_TR_FUNCTIONS(PackAction)

public:
	PackAction();

	virtual bool prepare(const FilesList &files);
	virtual void process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const;
};

ARC_PLUGIN_NS_END

#endif /* ARCPACKACTION_H_ */
