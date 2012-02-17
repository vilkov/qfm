#ifndef FILESYSTEMINFO_H_
#define FILESYSTEMINFO_H_

#include "../tools/filesystemfileinfo.h"
#include "../interfaces/filesystemifilecontrol.h"


FILE_SYSTEM_NS_BEGIN

class Info : public IFileControl
{
public:
    Info();
    Info(const QString &filePath);
    Info(const QString &filePath, bool identifyType);

	/* IFileType */
	virtual FileTypeId id() const;
	virtual QIcon icon() const;
	virtual QString name() const;
	virtual QString description() const;

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual bool exists() const;
	virtual IFile::size_type fileSize() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;
	virtual void refresh();

	/* IFileControl */
	virtual IFile::size_type freeSpace() const;
	virtual bool contains(const QString &fileName) const;
	virtual bool rename(const QString &newFileName, QString &error) const;

	virtual IFile *file(IFile::OpenMode mode, QString &error) const;
	virtual IFileControl *openFile(const QString &fileName, QString &error) const;
	virtual IFileControl *openFolder(const QString &fileName, bool create, QString &error) const;

public:
	bool isRoot() const { return m_isRoot; }
	bool shouldBeUpdatedBy(const Info &other) const
	{
		return
			m_info.lastModified != other.m_info.lastModified ||
			m_info.size != other.m_info.size ||
			m_info.type.name.isEmpty();
	}

private:
    bool m_isRoot;
    QString m_filePath;
    QString m_fileName;
    FileInfo m_info;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINFO_H_ */
