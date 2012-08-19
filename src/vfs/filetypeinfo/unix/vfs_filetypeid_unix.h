#ifndef VFS_FILETYPEID_UNIX_H_
#define VFS_FILETYPEID_UNIX_H_

#include <QtCore/QByteArray>
#include "../../vfs_ns.h"


VFS_NS_BEGIN

struct FileTypeId_Unix
{
	FileTypeId_Unix()
	{}

	bool operator==(const FileTypeId_Unix &other) const { return mime == other.mime; }
	bool operator<(const FileTypeId_Unix &other) const { return mime < other.mime; }
	bool isValid() const { return !mime.isEmpty(); }

	QByteArray mime;
};

VFS_NS_END

#endif /* VFS_FILETYPEID_UNIX_H_ */
