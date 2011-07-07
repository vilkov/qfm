#ifndef FILESYSTEMIFILECONTROL_H_
#define FILESYSTEMIFILECONTROL_H_

#include <QtCore/QString>
#include "filesystemifile.h"
#include "filesystemifileinfo.h"
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class IFileControl : public IFileInfo
{
public:
	enum FileType
	{
		File,
		Folder
	};
	enum Error
	{
		OK,
		Failed
	};

public:
	virtual bool contains(IFileControl *info) const = 0;
	virtual bool rename(const QString &newFileName, QString &error) const = 0;

	virtual IFile *open(IFile::OpenMode mode, QString &error) const = 0;
	virtual IFileControl *open(IFileControl *info, QString &error) const = 0;

	virtual IFileControl *create(IFileControl *info, QString &error) const = 0;
	virtual IFileControl *create(const QString &name, FileType type, QString &error) const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILECONTROL_H_ */
