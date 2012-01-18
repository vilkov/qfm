#ifndef ARCNODE_H_
#define ARCNODE_H_

#include "arcnodeproxymodel.h"
#include "items/arcnodeitem.h"
#include "../model/arcdelegate.h"
#include "../archive/arcarchive.h"
#include "../tasks/arctaskevent.h"
#include "../../../filesystem/info/filesysteminfo.h"
#include "../../../filesystem/tasks/filesystemtasksnode.h"
#include "../../../tools/containers/union.h"


ARC_PLUGIN_NS_BEGIN

class ArcNode : public TasksNode
{
public:
	ArcNode(const Info &info, Node *parent = 0);

	/* QObject */
    virtual bool event(QEvent *event);

    /* FileSystemModel */
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	/* IFileType */
	virtual FileTypeId id() const;
	virtual QIcon icon() const;
	virtual QString name() const;
	virtual QString description() const;

	/* IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool isLink() const;
	virtual bool exists() const;
	virtual qint64 fileSize() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual int permissions() const;
	virtual void refresh();

	/* IFileOperations */
	virtual IFileInfo *info(const QModelIndex &idx) const;
	virtual ICopyControl *createControl(INodeView *view) const;
	virtual void menuAction(QAction *action, INodeView *view);
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

	/* INode */
	virtual QAbstractItemModel *model() const;
	virtual QAbstractItemDelegate *delegate() const;
	virtual const INodeView::MenuActionList &actions() const;
	virtual QAbstractItemView::SelectionMode selectionMode() const;

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);

protected:
	/* TasksNode */
	virtual void updateProgressEvent(const TaskNodeItem *item, quint64 progress, quint64 timeElapsed);
	virtual void completedProgressEvent(const TaskNodeItem *item, quint64 timeElapsed);

	void scanCompleteEvent(TaskEvent *event);
	void copyCompleteEvent(TaskEvent *event);

private:
	enum { RootItemIndex = 0 };

	class ItemsContainer : public ModelContainer
	{
	public:
		typedef QList<ArcNodeItem *> List;

	public:
		ItemsContainer();
		virtual ~ItemsContainer();

		virtual size_type size() const;
		virtual Item *at(size_type index) const;
		virtual size_type indexOf(Item *item) const;

	private:
		friend class ArcNode;
		List m_container;
	};

private:
	typedef ::Tools::Containers::Union Union;
	void updateFirstColumn(ArcNodeItem *entry);
	void updateSecondColumn(ArcNodeItem *entry);

private:
	ItemsContainer m_itemsContainer;
    ItemsContainer::List &m_items;
    ArcNodeProxyModel m_proxy;
    ArcDelegate m_delegate;
    Info m_info;
    INodeView::MenuActionList m_actions;
};

ARC_PLUGIN_NS_END

#endif /* ARCNODE_H_ */
