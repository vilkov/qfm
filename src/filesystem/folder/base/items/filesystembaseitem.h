#ifndef FILESYSTEMBASEITEM_H_
#define FILESYSTEMBASEITEM_H_

#include "../../../info/filesysteminfo.h"
#include "../../../model/items/filesystemitem.h"


FILE_SYSTEM_NS_BEGIN

class FileSystemBaseItem : public FileSystemItem
{
public:
	FileSystemBaseItem(const Info &info, Item *parent = 0);

	virtual bool isRootItem() const = 0;
	virtual bool isListItem() const = 0;

	const Info &info() const { return m_info; }
	Info &info() { return m_info; }

	void setInfo(const Info &info) { m_info = info; }

private:
	class ItemInfo : public Info
	{
	public:
		ItemInfo(const Info &info);

		void operator=(const Info &info) { Info::operator=(info); }

		/* IFileInfo */
		virtual QString fileName() const;
	};

private:
	ItemInfo m_info;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMBASEITEM_H_ */
