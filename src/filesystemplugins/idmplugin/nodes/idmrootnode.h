#ifndef IDMROOTNODE_H_
#define IDMROOTNODE_H_

#include <sqlite3.h>
#include <QtCore/QFileInfo>
#include "idmdelegate.h"
#include "idmproxymodel.h"
#include "idmbasenode.h"
#include "items/idmitem.h"


FILE_SYSTEM_NS_BEGIN

class IdmRootNode : public IdmBaseNode
{
public:
	IdmRootNode(const QFileInfo &storage, Node *parent = 0);
	virtual ~IdmRootNode();

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	/* INode */
	virtual INode *root() const;
	virtual int columnCount() const;
	virtual IFileInfo *info(const QModelIndex &idx) const;
	virtual IFileControl *createControl() const;
	virtual IFileControl *createControl(const QModelIndex &idx, PluginsManager *plugins);

	/* INode::IFileInfo */
	virtual bool isDir() const;
	virtual bool isFile() const;
	virtual bool exists() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual void refresh();

	/* INode::IFileOperations */
	virtual void remove(const QModelIndexList &list);
	virtual void cancel(const QModelIndexList &list);
	virtual void calculateSize(const QModelIndexList &list);
	virtual void pathToClipboard(const QModelIndexList &list);
	virtual void copy(const QModelIndexList &list, INode *destination);
	virtual void move(const QModelIndexList &list, INode *destination);

	/* INode::IFileNavigation */
	virtual void viewClosed(INodeView *nodeView);
	virtual void viewParent(INodeView *nodeView);
	virtual void viewThis(INodeView *nodeView, const QModelIndex &selected);
	virtual void viewChild(INodeView *nodeView, const QModelIndex &idx, PluginsManager *plugins);
	virtual void viewChild(INodeView *nodeView, const Path::Iterator &path, PluginsManager *plugins);
	virtual void viewAbsolute(INodeView *nodeView, const QString &absoluteFilePath, PluginsManager *plugins);

	/* Node */
	virtual void setParentEntryIndex(const QModelIndex &value) { m_parentEntryIndex = value; }
	virtual void switchTo(Node *node, const QModelIndex &selected);

protected:
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }

private:
	QModelIndex rootIndex() const;
	void addView(INodeView *view);
	void removeView(INodeView *view);

private:
	IdmItem *rootItem() const { return m_items.at(0); }

private:
	typedef QSet<INodeView*> ViewSet;
	typedef QList<IdmItem*>  ItemsList;

private:
	sqlite3 *m_db;
	ViewSet m_view;
	bool m_updating;
	ItemsList m_items;
	IdmProxyModel m_proxy;
	IdmDelegate m_delegate;
	QModelIndex m_parentEntryIndex;
};

FILE_SYSTEM_NS_END

#endif /* IDMROOTNODE_H_ */
