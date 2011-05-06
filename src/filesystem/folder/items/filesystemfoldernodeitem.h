#ifndef FILESYSTEMITEMFOLDERNODEITEM_H_
#define FILESYSTEMITEMFOLDERNODEITEM_H_

#include <QtCore/QVariant>
#include "../../filesystem_ns.h"
#include "../../interfaces/filesystemifileinfo.h"


FILE_SYSTEM_NS_BEGIN

class FolderNodeItem : public IFileInfo
{
public:
	virtual QVariant data(qint32 column, qint32 role) const = 0;
	virtual bool isRoot() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMITEMFOLDERNODEITEM_H_ */
