#ifndef FILESYSTEMIFILECONTROL_H_
#define FILESYSTEMIFILECONTROL_H_

#include <QtCore/QString>
#include "filesystemifile.h"
#include "filesystemifileinfo.h"


FILE_SYSTEM_NS_BEGIN

class IFileControl : public IFileInfo
{
public:
	enum Error
	{
		OK,
		Failed
	};

public:
	virtual bool isPhysicalFile() const = 0;
	virtual IFile::size_type freeSpace() const = 0;
	virtual bool contains(const QString &fileName) const = 0;
	virtual bool rename(const QString &newFileName, QString &error) const = 0;

	virtual IFile *file(IFile::OpenMode mode, QString &error) const = 0;
	virtual IFileControl *openFile(const QString &fileName, QString &error) const = 0;
	virtual IFileControl *openFolder(const QString &fileName, bool create, QString &error) const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILECONTROL_H_ */
