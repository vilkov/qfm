#ifndef VFS_IFILECONTAINER_H_
#define VFS_IFILECONTAINER_H_

#include "vfs_ifileaccessor.h"
#include "vfs_ifilecontainerscanner.h"
#include "../location/vfs_location.h"


VFS_NS_BEGIN
class ICopyControl;
class INodeView;


class IFileContainer
{
public:
	typedef PScopedPointer<IFileContainer> Holder;

public:
	virtual ~IFileContainer();

	virtual bool isDefault() const = 0;
	virtual IFileInfo::size_type freeSpace() const = 0;
	virtual ICopyControl *createControl(INodeView *view) const = 0;

	virtual const Location &location() const = 0;
	virtual Location location(const IFileInfo *info) const = 0;
	virtual Location location(const QString &fileName) const = 0;

	virtual bool contains(const QString &fileName) const = 0;
	virtual IFileInfo *info(const QString &fileName, QString &error) const = 0;

	virtual bool remove(const IFileInfo *info, QString &error) const = 0;
	virtual bool rename(const IFileInfo *info, const QString &fileName, QString &error) const = 0;
	virtual bool move(const IFileContainer *source, const IFileInfo *info, QString &error) const = 0;

	virtual IFileContainer *open() const = 0;
	virtual IFileContainer *open(const IFileInfo *info, QString &error) const = 0;
	virtual IFileAccessor *open(const IFileInfo *info, int flags, QString &error) const = 0;
	virtual IFileContainer *create(const QString &fileName, QString &error) const = 0;
	virtual IFileAccessor *create(const QString &fileName, int flags, QString &error) const = 0;

	virtual const IFileContainerScanner *scanner() const = 0;

protected:
	static Location location(const QString &label, const QByteArray &location)
	{
		return Location(label, location);
	}
};

VFS_NS_END

#endif /* VFS_IFILECONTAINER_H_ */