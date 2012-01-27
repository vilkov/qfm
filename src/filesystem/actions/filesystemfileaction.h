#ifndef FILESYSTEMFILEACTION_H_
#define FILESYSTEMFILEACTION_H_

#include <QtGui/QAction>
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class FileAction
{
public:
	virtual ~FileAction();

	virtual bool isAsynchronous() const = 0;
	virtual const QAction *action() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILEACTION_H_ */
