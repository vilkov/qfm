#ifndef IDMROOTNODE_H_
#define IDMROOTNODE_H_

#include "idmdelegate.h"
#include "idmproxymodel.h"
#include "items/idmitem.h"
#include "../storage/idmstorage.h"
#include "../../../filesystem/filesystemnode.h"


FILE_SYSTEM_NS_BEGIN

class IdmRootNode : public Node
{
public:
	IdmRootNode(const QFileInfo &storage, Node *parent = 0);

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	/* INode */
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

	/* Node */
	virtual void switchTo(Node *node, const QModelIndex &selected);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual QAbstractItemModel *proxyModel() const { return &((IdmRootNode *)this)->m_proxy; }
	virtual QAbstractItemDelegate *itemDelegate() const { return &((IdmRootNode *)this)->m_delegate; }

	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);

protected:
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }

private:
	IdmItem *rootItem() const { return m_items.at(0); }

private:
	typedef QList<IdmItem*> ItemsList;

private:
	mutable QFileInfo m_info;
	IdmStorage m_storage;
	bool m_updating;
	ItemsList m_items;
	IdmProxyModel m_proxy;
	IdmDelegate m_delegate;
};

FILE_SYSTEM_NS_END

#endif /* IDMROOTNODE_H_ */
