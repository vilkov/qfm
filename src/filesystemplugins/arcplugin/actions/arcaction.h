#ifndef ARCACTION_H_
#define ARCACTION_H_

#include "../arcplugin_ns.h"
#include "../../../filesystem/actions/asynchronous/filesystemasyncfileaction.h"


ARC_PLUGIN_NS_BEGIN

class Action : public AsyncFileAction
{
public:
	Action(const QString &lockReason, const QIcon &icon, const QString &text);

	virtual const QString &lockReason() const;

private:
	QString m_lockReason;
};

ARC_PLUGIN_NS_END

#endif /* ARCACTION_H_ */
