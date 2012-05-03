#ifndef FILESYSTEMROOTNODE_H_
#define FILESYSTEMROOTNODE_H_

#include "../filesystemnode.h"


FILE_SYSTEM_NS_BEGIN

class RootNode : public Node
{
	Q_DISABLE_COPY(RootNode)

public:
	RootNode();
	virtual ~RootNode();

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

	/* IFileLocation */
	virtual QString location() const;
	virtual QString location(const QString &fileName) const;

	/* INode */
    virtual void refresh();
	virtual QString title() const;
	virtual Sorting sorting() const;
	virtual Geometry geometry() const;
	virtual QAbstractItemModel *model() const;
	virtual QAbstractItemDelegate *delegate() const;
	virtual const INodeView::MenuActionList &actions() const;
	virtual ::History::Entry *menuAction(QAction *action, INodeView *view);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);

private:
	class Container : public Node::Container
	{
	public:
		virtual size_type size() const;
		virtual Item *at(size_type index) const;
		virtual size_type indexOf(Item *item) const;
	};

private:
	Container m_items;
	INodeView::MenuActionList m_menuActions;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMROOTNODE_H_ */
