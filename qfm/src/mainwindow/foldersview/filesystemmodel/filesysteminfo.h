#ifndef EXTENDEDINFORMATION_H_
#define EXTENDEDINFORMATION_H_

#include <QString>
#include <QFileInfo>
#include <QFile>
#include <QIcon>
#include "../../../tools/memory/memory_manager.h"


class FileSystemInfo : public QFileInfo, public MemoryManagerTag
{
public:
    FileSystemInfo();
    FileSystemInfo(const QString &filePath);
    FileSystemInfo(const QFileInfo &info);

    bool operator==(const FileSystemInfo &fileInfo) const;

    void refresh();

    const QIcon &icon() const;
    const QString &displayType() const;
    bool isCaseSensitive() const;

    const QFile::Permissions &permissions() const { return m_permissions; }
    void setPermissions (QFile::Permissions permissions) { m_permissions = permissions; }

private:
    QFile::Permissions m_permissions;
    mutable QString m_displayType;
    mutable QIcon m_icon;
};

#endif /* EXTENDEDINFORMATION_H_ */
