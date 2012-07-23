#ifndef DEFAULTSEARCHNODE_H_
#define DEFAULTSEARCHNODE_H_

#include "../base/defaultbasenode.h"


DEFAULT_PLUGIN_NS_BEGIN

class SearchNode : public BaseNode
{
public:
	SearchNode(IFileContainer::Holder &container, IFileContainerScanner::Filter::Holder &filter, Node *parent = 0);

	/* QObject */
    virtual bool event(QEvent *event);

	/* IFileOperations */
	virtual ICopyControl *createControl(INodeView *view) const;
	virtual void contextMenu(const QModelIndexList &list, INodeView *view);
	virtual void createFile(const QModelIndex &index, INodeView *view);
	virtual void createDirectory(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndex &index, INodeView *view);
	virtual void rename(const QModelIndexList &list, INodeView *view);
	virtual void remove(const QModelIndexList &list, INodeView *view);
	virtual void cancel(const QModelIndexList &list, INodeView *view);
	virtual void calculateSize(const QModelIndexList &list, INodeView *view);
	virtual void pathToClipboard(const QModelIndexList &list, INodeView *view);
	virtual void copy(const INodeView *source, INodeView *destination);
	virtual void move(const INodeView *source, INodeView *destination);
	virtual void removeToTrash(const QModelIndexList &list, INodeView *view);
	virtual ::History::Entry *search(const QModelIndex &index, INodeView *view);

	/* INode */
    virtual void refresh();

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual Node *viewChild(const QModelIndex &idx, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, QModelIndex &selected);

protected:
	/* TasksNode */
	void searchNewFileEvent(BaseTask::Event *event);
	void searchCompleteEvent(BaseTask::Event *event);

private:
	enum { RootItemIndex = 0 };

private:
	typedef ::Tools::Containers::Union Union;
	void updateFirstColumn();
	void updateSecondColumn();
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTSEARCHNODE_H_ */
