#ifndef FILESYSTEMBASEITEM_H_
#define FILESYSTEMBASEITEM_H_

#include "../../../info/filesysteminfo.h"
#include "../../../model/items/filesystemitem.h"


FILE_SYSTEM_NS_BEGIN

class FileSystemBaseItem : public FileSystemItem, public IFileInfo
{
public:
	FileSystemBaseItem(const Info &info, Item *parent = 0);

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool exists() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual void refresh();

	virtual bool isRootItem() const = 0;
	virtual bool isListItem() const = 0;

	const Info &info() const { return m_info; }
	void setInfo(const Info &info) { m_info = info; }

private:
	Info m_info;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMBASEITEM_H_ */
