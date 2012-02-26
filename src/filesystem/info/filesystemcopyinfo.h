#ifndef FILESYSTEMCOPYINFO_H_
#define FILESYSTEMCOPYINFO_H_

#include <QtCore/QCoreApplication>
#include "filesysteminfo.h"
#include "../tasks/interfaces/filesystemicopycontrol.h"


FILE_SYSTEM_NS_BEGIN

class CopyInfo : public ICopyControl
{
	Q_DECLARE_TR_FUNCTIONS(CopyInfo)

public:
	CopyInfo(const Info &info);

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
	virtual bool isPhysicalFile() const;
	virtual IFile::size_type freeSpace() const;
	virtual bool contains(const QString &fileName) const;
	virtual bool rename(const QString &newFileName, QString &error) const;

	virtual IFile *file(IFile::OpenMode mode, QString &error) const;
	virtual IFileControl *openFile(const QString &fileName, QString &error) const;
	virtual IFileControl *openFolder(const QString &fileName, bool create, QString &error) const;

	/* ICopyControl */
	virtual bool start(const ScanedFiles::Files &files, bool move);
	virtual void done(bool error);
	virtual void canceled();

private:
	Info m_info;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMCOPYINFO_H_ */
