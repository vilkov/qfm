#ifndef DEFAULTINVALIDINFO_H_
#define DEFAULTINVALIDINFO_H_

#include <vfs/interfaces/vfs_ifileinfo.h>
#include "../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class InvalidInfo : public IFileInfo, public IFileType
{
public:
	InvalidInfo(const QString &fileName);

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
	virtual const IFileType *fileType() const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;

private:
	QString m_fileName;
	QIcon m_icon;
};

IDM_PLUGIN_NS_END

#endif /* DEFAULTINVALIDINFO_H_ */
