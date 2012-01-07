#ifndef FILESYSTEMINFO_H_
#define FILESYSTEMINFO_H_

#include <QtGui/QIcon>
#include "../tools/filesystemfileinfo.h"
#include "../interfaces/filesystemifilecontrol.h"
#include "../../de/filesystemfiletypeinfo.h"


FILE_SYSTEM_NS_BEGIN

class Info : public IFileControl
{
public:
    Info();
    Info(const QString &filePath);
    Info(const QString &filePath, bool identifyType);

	/* IFileType */
	virtual QIcon icon() const;
	virtual QString name() const;
	virtual QString description() const;

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual bool exists() const;
	virtual qint64 fileSize() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;
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
	bool isRoot() const { return m_isRoot; }
	bool shouldBeUpdatedBy(const Info &other) const
	{
		return
			m_info.lastModified != other.m_info.lastModified ||
			m_info.size != other.m_info.size ||
			m_typeInfo.mimeType.isEmpty();
	}

private:
    bool m_isRoot;
    QString m_filePath;
    QString m_fileName;
    FileInfo m_info;
    FileTypeInfo m_typeInfo;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINFO_H_ */
