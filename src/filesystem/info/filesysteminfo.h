#ifndef FILESYSTEMINFO_H_
#define FILESYSTEMINFO_H_

#include <QtCore/QFileInfo>
#include <QtGui/QIcon>
#ifndef Q_OS_WIN
#  include <unistd.h>
#  include <sys/types.h>
#endif
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class Info : public QFileInfo
{
public:
    Info();
    Info(const QString &filePath);
    Info(const QFileInfo &info);
#ifndef Q_OS_WIN
    Info(const QString &filePath, uint userId, uint groupId);
    Info(const QFileInfo &info, uint userId, uint groupId);
#endif

    bool operator==(const Info &fileInfo) const;

    void refresh();

    const QIcon &icon() const;
    const QString &displayType() const;
    bool isCaseSensitive() const;

    const QFile::Permissions &permissions() const { return m_permissions; }

private:
#ifndef Q_OS_WIN
    void translatePermissions(uint userId, uint groupId);
#endif

private:
    QFile::Permissions m_permissions;
    mutable QString m_displayType;
    mutable QIcon m_icon;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINFO_H_ */
