#ifndef FILESYSTEMROOTITEM_H_
#define FILESYSTEMROOTITEM_H_

#include "../base/items/filesystembaseitem.h"


FILE_SYSTEM_NS_BEGIN

class FilesystemRootItem : public FileSystemBaseItem
{
public:
	explicit FilesystemRootItem();

	/* IFileInfo */
	virtual QString fileName() const;

	/* ::Tools::Models::Tree::Item */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* FileSystemBaseItem */
	virtual bool isRootItem() const;
	virtual bool isListItem() const;

private:
	QVariant m_label;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMROOTITEM_H_ */
