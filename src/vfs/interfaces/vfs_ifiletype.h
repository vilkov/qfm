#ifndef VFS_IFILETYPE_H_
#define VFS_IFILETYPE_H_

#include <QtCore/QString>
#include <QtGui/QIcon>
#include "../vfs_ns.h"
#include "../filetypeinfo/vfs_filetypeinfo.h"


VFS_NS_BEGIN

class IFileType
{
public:
	virtual ~IFileType();

	virtual FileTypeId id() const = 0;
	virtual QIcon icon() const = 0;
	virtual QString name() const = 0;
	virtual QString description() const = 0;
};

VFS_NS_END

#endif /* VFS_IFILETYPE_H_ */
