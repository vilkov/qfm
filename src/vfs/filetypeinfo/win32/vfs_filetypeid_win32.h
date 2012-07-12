#ifndef VFS_FILETYPEID_WIN32_H_
#define VFS_FILETYPEID_WIN32_H_

#include <QtCore/QString>
#include "../../vfs_ns.h"


VFS_NS_BEGIN

struct FileTypeId_Win32
{
	FileTypeId_Win32();

	bool operator==(const FileTypeId_Win32 &other) const;
	bool operator<(const FileTypeId_Win32 &other) const;

	QString extension;
    QIcon icon;
};

VFS_NS_END

#endif /* VFS_FILETYPEID_WIN32_H_ */
