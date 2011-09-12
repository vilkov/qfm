#ifndef FILESYSTEMINFO_H_
#define FILESYSTEMINFO_H_

#include <QtCore/QFileInfo>
#include <QtGui/QIcon>
#include "../interfaces/filesystemifilecontrol.h"


FILE_SYSTEM_NS_BEGIN

class Info : public IFileControl
{
public:
    Info();
    Info(const QString &filePath);
    Info(const QFileInfo &info);
#ifndef Q_OS_WIN
    Info(const QString &filePath, uint userId, uint groupId);
    Info(const QFileInfo &info, uint userId, uint groupId);
#endif

	/* IFileControl::IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool exists() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual void refresh();

	/* IFileControl */
	virtual IFile::size_type freeSpace() const;
	virtual bool contains(IFileControl *info) const;
	virtual bool rename(const QString &newFileName, QString &error) const;

	virtual IFile *open(IFile::OpenMode mode, QString &error) const;
	virtual IFileControl *open(IFileControl *info, QString &error) const;

	virtual IFileControl *create(IFileControl *info, QString &error) const;
	virtual IFileControl *create(const QString &name, FileType type, QString &error) const;

public:
	bool isRoot() const { return m_info.isRoot(); }
	qint64 size() const { return m_info.size(); }
    const QIcon &icon() const;
    const QString &displayType() const;
    bool isCaseSensitive() const;

    const QFile::Permissions &permissions() const { return m_permissions; }

private:
#ifndef Q_OS_WIN
    void translatePermissions(uint userId, uint groupId);
#endif

private:
    QFileInfo m_info;
    QFile::Permissions m_permissions;
    mutable QString m_displayType;
    mutable QIcon m_icon;
#ifndef Q_OS_WIN
    static QString root;
#endif
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINFO_H_ */
