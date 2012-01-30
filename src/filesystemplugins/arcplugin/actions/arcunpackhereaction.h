#ifndef ARCUNPACKHEREACTION_H_
#define ARCUNPACKHEREACTION_H_

#include <QtCore/QCoreApplication>
#include "../arcplugin_ns.h"
#include "../../../filesystem/actions/asynchronous/filesystemasyncfileaction.h"


ARC_PLUGIN_NS_BEGIN

class UnPackHereAction : public AsyncFileAction
{
	Q_DECLARE_TR_FUNCTIONS(UnPackHereAction)

public:
	UnPackHereAction();

	virtual const QAction *action() const;
	virtual bool prepare(const FilesList &files);
	virtual void process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const;

private:
	QAction m_action;
};

ARC_PLUGIN_NS_END

#endif /* ARCUNPACKHEREACTION_H_ */
