#ifndef IDMINVALIDINFO_H_
#define IDMINVALIDINFO_H_

#include <vfs/interfaces/vfs_ifileinfo.h>
#include "../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class InvalidInfo : public IFileInfo, public IFileType
{
public:
	InvalidInfo(const QString &fileName);

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual size_type fileSize() const;
	virtual const Location &fileName() const;
	virtual const IFileType *fileType() const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;

	/* IFileType */
	virtual FileTypeId id() const;
	virtual QIcon icon() const;
	virtual QString name() const;
	virtual QString description() const;

private:
	Location m_fileName;
	QIcon m_icon;
};

IDM_PLUGIN_NS_END

#endif /* IDMINVALIDINFO_H_ */
