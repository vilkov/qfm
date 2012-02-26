#ifndef FILESYSTEMINVALIDINFO_H_
#define FILESYSTEMINVALIDINFO_H_

#include "../interfaces/filesystemifileinfo.h"


FILE_SYSTEM_NS_BEGIN

class InvalidInfo : public IFileInfo
{
public:
	InvalidInfo();

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
	virtual size_type fileSize() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;
	virtual void refresh();
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINVALIDINFO_H_ */
