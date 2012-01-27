#ifndef FILESYSTEMIFILEACTION_H_
#define FILESYSTEMIFILEACTION_H_

#include "filesystemifileoperations.h"
#include "filesystemifilenavigation.h"


FILE_SYSTEM_NS_BEGIN

class IFileAction : public IFileInfo, public IFileOperations, public IFileNavigation
{
public:
	virtual ~INode() {}

	virtual INode *root() const = 0;
	virtual int columnsCount() const = 0;

	virtual QAbstractItemModel *model() const = 0;
	virtual QAbstractItemDelegate *delegate() const = 0;
	virtual const INodeView::MenuActionList &actions() const = 0;
	virtual QAbstractItemView::SelectionMode selectionMode() const = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMIFILEACTION_H_ */
