#ifndef FILESYSTEMFILETYPEID_WIN32_H_
#define FILESYSTEMFILETYPEID_WIN32_H_

#include <QtCore/QString>
#include "../../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

struct FileTypeId_Win32
{
	FileTypeId_Win32();

	bool operator==(const FileTypeId_Win32 &other) const;
	bool operator<(const FileTypeId_Win32 &other) const;

	QString extension;
    QIcon icon;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILETYPEID_WIN32_H_ */
