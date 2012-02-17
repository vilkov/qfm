#ifndef ARCUNPACKINTOSUBDIRACTION_H_
#define ARCUNPACKINTOSUBDIRACTION_H_

#include <QtCore/QCoreApplication>
#include "arcaction.h"


ARC_PLUGIN_NS_BEGIN

class UnPackIntoSubdirAction : public Action
{
	Q_DECLARE_TR_FUNCTIONS(UnPackIntoSubdirAction)

public:
	UnPackIntoSubdirAction();

	virtual bool prepare(const FilesList &files);
	virtual void process(PerformActionTask *task, const FilesList &files, const volatile Flags &flags) const;

private:
	QString folderName(const QString &fileName) const;
};

ARC_PLUGIN_NS_END

#endif /* ARCUNPACKINTOSUBDIRACTION_H_ */
