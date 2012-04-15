#ifndef FILESYSTEMINODE_H_
#define FILESYSTEMINODE_H_

#include "filesystemifileoperations.h"
#include "filesystemifilenavigation.h"


FILE_SYSTEM_NS_BEGIN

class INode : public IFileOperations, public IFileNavigation
{
public:
	virtual ~INode();

    virtual void refresh() = 0;
	virtual QString title() const = 0;
	virtual QString location() const = 0;
	virtual QString location(const QString &fileName) const = 0;
	virtual QString location(const QModelIndex &index) const = 0;

	virtual int columnsCount() const = 0;
	virtual QAbstractItemModel *model() const = 0;
	virtual QAbstractItemDelegate *delegate() const = 0;
	virtual const INodeView::MenuActionList &actions() const = 0;
	virtual QAbstractItemView::SelectionMode selectionMode() const = 0;
	virtual ::History::Entry *menuAction(QAction *action, INodeView *view) = 0;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMINODE_H_ */
