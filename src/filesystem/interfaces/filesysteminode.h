#ifndef FILESYSTEMINODE_H_
#define FILESYSTEMINODE_H_

#include "filesystemifileoperations.h"
#include "filesystemifilenavigation.h"


FILE_SYSTEM_NS_BEGIN

class INode : public IFileInfo, public IFileOperations, public IFileNavigation
{
public:
	virtual ~INode() {}

	virtual int columnsCount() const = 0;

	virtual QAbstractItemModel *model() const = 0;
	virtual QAbstractItemDelegate *delegate() const = 0;
	virtual const INodeView::MenuActionList &actions() const = 0;
	virtual QAbstractItemView::SelectionMode selectionMode() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINODE_H_ */
