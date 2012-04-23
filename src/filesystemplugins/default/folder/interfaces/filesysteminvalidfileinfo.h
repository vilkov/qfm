#ifndef FILESYSTEMINVALIDINFO_H_
#define FILESYSTEMINVALIDINFO_H_

#include "../filesystemifileinfo.h"


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
	virtual size_type fileSize() const;
	virtual QString fileName() const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINVALIDINFO_H_ */
