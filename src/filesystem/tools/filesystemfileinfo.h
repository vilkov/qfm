#ifndef FILESYSTEMFILEINFO_H_
#define FILESYSTEMFILEINFO_H_

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtGui/QIcon>
#include "../filesystem_ns.h"


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
    QString type;
    QIcon icon;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFILEINFO_H_ */
