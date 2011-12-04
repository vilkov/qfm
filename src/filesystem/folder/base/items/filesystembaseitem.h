#ifndef FILESYSTEMBASEITEM_H_
#define FILESYSTEMBASEITEM_H_

#include "../../../filesystemnode.h"
#include "../../../info/filesysteminfo.h"
#include "../../../tasks/items/filesystemtasknodeitem.h"


FILE_SYSTEM_NS_BEGIN

class FileSystemBaseItem : public TaskNodeItem
{
public:
	FileSystemBaseItem(const Info &info, Item *parent = 0);

	/* FileSystemItem */
	virtual bool isList() const;

	Node *node() const { return m_node; }

	virtual bool isRootItem() const = 0;
	virtual bool isListItem() const = 0;

	const Info &info() const { return m_info; }
	Info &info() { return m_info; }

	void setInfo(const Info &info) { m_info = info; }

protected:
	void setNode(Node *node) { m_node = node; }

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
	Node *m_node;
	ItemInfo m_info;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMBASEITEM_H_ */
