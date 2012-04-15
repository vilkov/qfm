#ifndef FILESYSTEMROOTFOLDERITEM_H_
#define FILESYSTEMROOTFOLDERITEM_H_

#include "../base/items/filesystemfolderbaseitem.h"


FILE_SYSTEM_NS_BEGIN

class RootFolderItem : public FolderBaseItem
{
public:
	explicit RootFolderItem();

	/* IFileInfo */
	virtual QString fileName() const;

	/* ::Tools::Models::Tree::Item */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* FileSystemBaseItem */
	virtual bool isRootItem() const;

private:
	QVariant m_label;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMROOTFOLDERITEM_H_ */
