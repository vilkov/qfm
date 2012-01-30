#ifndef ARCUNPACKACTION_H_
#define ARCUNPACKACTION_H_

#include <QtCore/QCoreApplication>
#include "../arcplugin_ns.h"
#include "../../../filesystem/actions/asynchronous/filesystemasyncfileaction.h"


ARC_PLUGIN_NS_BEGIN

class UnPackAction : public AsyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(UnPackAction)

public:
	UnPackAction();

	virtual const QAction *action() const;
	virtual void process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const;

private:
	QAction m_action;
};

ARC_PLUGIN_NS_END

#endif /* ARCUNPACKACTION_H_ */
