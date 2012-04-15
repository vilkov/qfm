#ifndef FILESYSTEMFOLDERBASEITEM_H_
#define FILESYSTEMFOLDERBASEITEM_H_

#include "../../../filesystemnode.h"
#include "../../../interfaces/imp/filesystemfileinfo.h"
#include "../../../tasks/items/filesystemtasknodeitem.h"


FILE_SYSTEM_NS_BEGIN

class FolderBaseItem : public TaskNodeItem
{
public:
	FolderBaseItem(const Info &info, Base *parent = 0);

	Node *node() const { return m_node; }

	virtual bool isRootItem() const = 0;

	const Info &info() const { return m_info; }
	Info &info() { return m_info; }

	void setInfo(const Info &info) { m_info = info; }

protected:
	void setNode(Node *node) { m_node = node; }

private:
	Node *m_node;
	Info m_info;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERBASEITEM_H_ */
