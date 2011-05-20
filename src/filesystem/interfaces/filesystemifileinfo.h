#ifndef FILESYSTEMIFILEINFO_H_
#define FILESYSTEMIFILEINFO_H_

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include "filesystemifile.h"
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class IFileInfo
{
public:
	enum FileType
	{
		File,
		Folder
	};

public:
	virtual ~IFileInfo() {}

	virtual bool isDir() const = 0;
	virtual bool isFile() const = 0;
	virtual bool exists() const = 0;
	virtual QString fileName() const = 0;
	virtual QString absolutePath() const = 0;
	virtual QString absoluteFilePath() const = 0;
	virtual QString absoluteFilePath(const QString &fileName) const = 0;
	virtual QDateTime lastModified() const = 0;

	virtual IFile *open(IFile::OpenMode mode, QString &error) const = 0;
	virtual IFileInfo *create(const QString &fileName, FileType type, QString &error) const = 0;

	virtual void refresh() = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILEINFO_H_ */
