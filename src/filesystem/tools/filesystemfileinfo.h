#ifndef FILESYSTEMFILEINFO_H_
#define FILESYSTEMFILEINFO_H_

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include "../filesystem_ns.h"
#include "../filetypeinfo/filetypeinfo.h"


FILE_SYSTEM_NS_BEGIN

struct FileInfo
{
	FileInfo();

    bool isDir;
    bool isFile;
    bool isLink;
    int permissions;
    qint64 size;
    QDateTime lastModified;
    FileTypeInfo type;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILEINFO_H_ */
