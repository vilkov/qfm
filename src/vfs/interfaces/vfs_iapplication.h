#ifndef VFS_IAPPLICATION_H_
#define VFS_IAPPLICATION_H_

#include <QtGui/QIcon>
#include <QtCore/QString>
#include "../vfs_ns.h"


VFS_NS_BEGIN
class IFileInfo;
class IFileContainer;


class IApplication
{
public:
	virtual ~IApplication();

	virtual const QIcon &icon() const = 0;
	virtual const QString &name() const = 0;
	virtual const QString &description() const = 0;

	virtual bool exec(const IFileContainer *container, const IFileInfo *file, QString &error) const = 0;
};

VFS_NS_END

#endif /* VFS_IAPPLICATION_H_ */
