#ifndef DEFAULT_SEARCHNODE_H_
#define DEFAULT_SEARCHNODE_H_

#include "../base/default_basenode.h"


DEFAULT_PLUGIN_NS_BEGIN

class SearchNode : public TasksNode
{
public:
	SearchNode(IFileContainer::Holder &container, IFileContainerScanner::Filter::Holder &filter, Node *parent = 0);

	/* QObject */
    virtual bool event(QEvent *event);

    /* Model */
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	/* INode */
    virtual void refresh();
	virtual QString location() const;
	virtual QString title() const;
	virtual bool shortcut(INodeView *view, QKeyEvent *event);

	virtual QAbstractItemModel *model() const;
	virtual QAbstractItemDelegate *delegate() const;
	virtual const INodeView::MenuActionList &actions() const;
	virtual void menuAction(INodeView *view, QAction *action);

	/* IFileOperations */
	virtual ICopyControl *createControl(INodeView *view) const;
	virtual void contextMenu(const QModelIndexList &list, INodeView *view);
	virtual void cancel(const QModelIndexList &list, INodeView *view);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual Node *viewChild(const QModelIndex &idx, QModelIndex &selected, bool newTab);
	virtual Node *viewChild(const QString &fileName, QModelIndex &selected);

protected:
	/* TasksNode */
	virtual void updateProgressEvent(const Item::Holder &item, quint64 progress, quint64 timeElapsed);
	virtual void completedProgressEvent(const Item::Holder &item, quint64 timeElapsed);
	virtual void performActionEvent(const AsyncFileAction::FilesList &files, const QString &error);

	void searchNewFileEvent(BaseTask::Event *event);
	void searchCompleteEvent(BaseTask::Event *event);

protected:
	virtual void copy(const INodeView *source, INodeView *destination);
	virtual void move(const INodeView *source, INodeView *destination);

protected:
	class Container : public TasksNode::Container
	{
	public:
		typedef QList<NodeItem::Holder> List;

	public:
		virtual size_type size() const;
		virtual Item *at(size_type index) const;
		virtual size_type indexOf(Item *item) const;

		List list;
	};

private:
	enum { RootItemIndex = 0 };

private:
	typedef ::Tools::Containers::Union Union;
	void updateFirstColumn(Container::size_type index, NodeItem *entry);
	void updateSecondColumn(Container::size_type index, NodeItem *entry);

private:
	enum ShortcutType
	{
		NoShortcut,

		CopyShortcut,
		MoveShortcut,

		SizeOf_ShortcutType
	};

	typedef QMap<quint32, ShortcutType> Shortcuts;

private:
	IFileContainer::Holder m_container;
	Shortcuts m_shortcuts;
	Container m_items;
	ProxyModel m_proxy;
	Delegate m_delegate;
	INodeView::MenuActionList m_menuActions;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_SEARCHNODE_H_ */
