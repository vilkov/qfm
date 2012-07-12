#ifndef VFS_FILETYPEINFO_H_
#define VFS_FILETYPEINFO_H_

#include <QtGui/QIcon>

#if !defined(DESKTOP_ENVIRONMENT_IS_WINDOWS)
#	include "unix/vfs_filetypeid_unix.h"
	VFS_NS(typedef ::VFS::FileTypeId_Unix FileTypeId)
#else
#	include "win32/vfs_filetypeid_win32.h"
	VFS_NS(typedef ::VFS::FileTypeId_Win32 FileTypeId)
#endif


VFS_NS_BEGIN

struct FileTypeInfo
{
	FileTypeInfo()
	{}

	bool isValid() const { return id.isValid(); }

	FileTypeId id;
	QIcon icon;
	QString name;
	QString description;
};

VFS_NS_END

#endif /* VFS_FILETYPEINFO_H_ */
