#ifndef EXTENDEDINFORMATION_H_
#define EXTENDEDINFORMATION_H_

#include <QString>
#include <QFSFileEngine>
#include <QFileInfo>
#include <QDateTime>
#include <QFile>
#include <QIcon>
#include "../../../tools/memory/memory_manager.h"


class FileSystemInfo : public MemoryManagerTag
{
public:
    enum Type
    {
    	Dir,
    	File,
    	System,
    	Source
    };

    FileSystemInfo()
    {}
    FileSystemInfo(const QFileInfo &info) :
    	m_fileInfo(info),
    	m_type(type())
    {}

    bool operator ==(const FileSystemInfo &fileInfo) const
    {
       return m_fileInfo == fileInfo.m_fileInfo &&
       m_displayType == fileInfo.m_displayType &&
       permissions() == fileInfo.permissions();
    }

    inline bool isDir() const { return m_type == Dir; }
    inline bool isFile() const { return m_type == File; }
    inline bool isSystem() const { return m_type == System; }
    inline bool isSource() const { return m_type == Source; }
    inline bool isSymLink() const { return m_fileInfo.isSymLink(); }
    inline bool isHidden() const { return m_fileInfo.isHidden(); }
    inline bool isCaseSensitive() const { return QFSFileEngine(m_fileInfo.absoluteFilePath()).caseSensitive(); }

    const QFileInfo &fileInfo() const { return m_fileInfo; }
    QFileInfo &fileInfo() { return m_fileInfo; }
    const QFile::Permissions &permissions() const { return m_permissions; }
    const QString &displayType() const { return m_displayType; }
    const QIcon &icon() const { return m_icon; }

    void setPermissions (QFile::Permissions permissions) { m_permissions = permissions; }
    void setDisplayType(const QString &displayType) { m_displayType = displayType; }
    void setIcon(const QIcon &icon) { m_icon = icon; }

private:
    Type type() const
    {
        if (m_fileInfo.isDir())
            return Dir;

        if (m_fileInfo.isFile())
        	if (isSourceExtension(m_fileInfo.fileName().mid(m_fileInfo.fileName().lastIndexOf(L'.'))))
        		return Source;
        	else
        		return File;

        if (!m_fileInfo.exists() && m_fileInfo.isSymLink())
            return System;

        return System;
    }

    static bool isSourceExtension(const QString &extension)
    {
    	return extension == QString::fromLatin1(".h")   ||
    		   extension == QString::fromLatin1(".cpp") ||
    		   extension == QString::fromLatin1(".hpp") ||
    		   extension == QString::fromLatin1(".hxx") ||
    		   extension == QString::fromLatin1(".cxx");
    }

private:
    QFileInfo m_fileInfo;
    QFile::Permissions m_permissions;
    QString m_displayType;
    QIcon m_icon;
    Type m_type;
};

#endif /* EXTENDEDINFORMATION_H_ */
