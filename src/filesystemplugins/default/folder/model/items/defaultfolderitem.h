#ifndef DEFAULTFOLDERITEM_H_
#define DEFAULTFOLDERITEM_H_

#include "../../interfaces/folderfileinfo.h"
#include "../../../model/items/defaultnodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN

class DefaultFolderItem : public DefaultNodeItem
{
public:
	DefaultFolderItem(const Info &info, Base *parent = 0);
	DefaultFolderItem(const Info &info, Node *node, Base *parent = 0);

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual IFileInfo::size_type fileSize() const;
	virtual QString fileName() const;
	virtual const IFileType *fileType() const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;

private:
	Info m_info;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFOLDERITEM_H_ */
