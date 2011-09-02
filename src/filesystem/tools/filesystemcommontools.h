#ifndef FILESYSTEMCOMMONTOOLS_H_
#define FILESYSTEMCOMMONTOOLS_H_

#include <QtCore/QString>
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

struct Tools
{
	static QString humanReadableTime(quint64 msecs);
	static QString humanReadableSize(quint64 size);
	static QString humanReadableShortSize(quint64 size);
	static quint64 freeSpace(const QByteArray &utf8AbsoluteFolderPath);
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMCOMMONTOOLS_H_ */
