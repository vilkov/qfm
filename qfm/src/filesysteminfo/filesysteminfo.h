#ifndef EXTENDEDINFORMATION_H_
#define EXTENDEDINFORMATION_H_

#include <QtCore/QFileInfo>
#include <QtGui/QIcon>
#ifndef Q_OS_WIN
#  include <unistd.h>
#  include <sys/types.h>
#endif
#include "../tools/memory/memory_manager.h"


class FileSystemInfo : public QFileInfo, public MemoryManagerTag
{
public:
    FileSystemInfo();
    FileSystemInfo(const QString &filePath);
    FileSystemInfo(const QFileInfo &info);
#ifndef Q_OS_WIN
    FileSystemInfo(const QString &filePath, uint userId, uint groupId);
    FileSystemInfo(const QFileInfo &info, uint userId, uint groupId);
#endif

    bool operator==(const FileSystemInfo &fileInfo) const;

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

#endif /* EXTENDEDINFORMATION_H_ */
