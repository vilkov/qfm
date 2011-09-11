#ifndef FILESYSTEMICOPYCONTROL_H_
#define FILESYSTEMICOPYCONTROL_H_

#include <QtCore/QList>
#include "filesystemifilecontrol.h"


FILE_SYSTEM_NS_BEGIN

class ICopyControl : public IFileControl
{
public:
	typedef QList<IFileInfo*> FileInfoList;

public:
	virtual bool acceptCopy(const FileInfoList &files, bool move) const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMICOPYCONTROL_H_ */
