#ifndef M3UNODE_H_
#define M3UNODE_H_

#include <QtCore/QFileInfo>
#include "m3uplugin_ns.h"
#include "m3udelegate.h"
#include "m3uproxymodel.h"
#include "items/m3uitem.h"
#include "../../filesystem/filesystemnode.h"
#include "../../filesystem/filesystempluginsmanager.h"


M3U_PLUGIN_NS_BEGIN

class M3uNode : public Node
{
public:
	M3uNode(const QFileInfo &info, Node *parent = 0);
	virtual ~M3uNode();

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

	/* INode */
	virtual IFileInfo *info(const QModelIndex &idx) const { return 0; }
	virtual IFileControl *acceptCopy(const FileInfoList &files, bool move) const { return 0; }

	/* INode::IFileInfo */
	virtual bool isDir() const { return false; }
	virtual bool isFile() const { return false; }
	virtual bool exists() const;
	virtual QString fileName() const;
	virtual QString absolutePath() const;
	virtual QString absoluteFilePath() const;
	virtual QString absoluteFilePath(const QString &fileName) const;
	virtual QDateTime lastModified() const;
	virtual void refresh();

	/* INode::IFileOperations */
	virtual void menuAction(QAction *action);
	virtual void createFile(const QModelIndex &index);
	virtual void createDirectory(const QModelIndex &index);
	virtual void rename(const QModelIndexList &list);
	virtual void remove(const QModelIndexList &list);
	virtual void cancel(const QModelIndexList &list);
	virtual void calculateSize(const QModelIndexList &list);
	virtual void pathToClipboard(const QModelIndexList &list);
	virtual void copy(const QModelIndexList &list, INode *destination);
	virtual void move(const QModelIndexList &list, INode *destination);

	/* Node */
	virtual void switchTo(Node *node, const QModelIndex &selected);

protected:
	virtual QModelIndex rootIndex() const { return QModelIndex(); }
	virtual QAbstractItemModel *proxyModel() const { return &((M3uNode *)this)->m_proxy; }
	virtual QAbstractItemDelegate *itemDelegate() const { return &((M3uNode *)this)->m_delegate; }
	virtual const INodeView::MenuActionList &menuActions() const { return m_menuActions; }

	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);

protected:
	M3uItem *rootItem() const { return m_items.at(0); }
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }

private:
	typedef QList<M3uItem*> ItemsList;

private:
	QString m_tag;
	bool m_updating;
	ItemsList m_items;
	M3uProxyModel m_proxy;
	M3uDelegate m_delegate;
	INodeView::MenuActionList m_menuActions;
};

M3U_PLUGIN_NS_END

#endif /* M3UNODE_H_ */
