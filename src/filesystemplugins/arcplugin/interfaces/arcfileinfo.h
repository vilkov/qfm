#ifndef ARCFILEINFO_H_
#define ARCFILEINFO_H_

#include "../arcplugin_ns.h"
#include "../../../filesystem/interfaces/filesystemifileinfo.h"


struct archive_entry;


ARC_PLUGIN_NS_BEGIN

class Info : public IFileInfo, public IFileType
{
public:
	Info(const QString &fileName, struct archive_entry *entry);

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual size_type fileSize() const;
	virtual QString fileName() const;
	virtual const IFileType *fileType() const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;

	/* IFileType */
	virtual FileTypeId id() const;
	virtual QIcon icon() const;
	virtual QString name() const;
	virtual QString description() const;
};

ARC_PLUGIN_NS_END

#endif /* ARCFILEINFO_H_ */
