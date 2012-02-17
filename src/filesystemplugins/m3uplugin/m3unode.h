#ifndef M3UNODE_H_
#define M3UNODE_H_

#include <QtCore/QFileInfo>
#include "m3uplugin_ns.h"
#include "m3udelegate.h"
#include "m3uproxymodel.h"
#include "containers/m3ucontainer.h"
#include "../../filesystem/filesystemnode.h"
#include "../../filesystem/filesystempluginsmanager.h"


M3U_PLUGIN_NS_BEGIN

class M3uNode : public Node
{
public:
	M3uNode(const QFileInfo &info, Node *parent = 0);

    /* QAbstractItemModel */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent) const;
	virtual QVariant data(const QModelIndex &index, int role) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex &child) const;

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
	virtual IFile::size_type fileSize() const;
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
	virtual void contextMenu(const QModelIndexList &list, INodeView *view);
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

	/* Node */
	virtual void switchViewsTo(Node *node, const QModelIndex &selected);

protected:
	virtual QModelIndex rootIndex() const { return QModelIndex(); }
	virtual QAbstractItemModel *proxyModel() const { return &((M3uNode *)this)->m_proxy; }
	virtual QAbstractItemDelegate *itemDelegate() const { return &((M3uNode *)this)->m_delegate; }
	virtual const INodeView::MenuActionList &menuActions() const { return m_menuActions; }

	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);

protected:
	M3uItem *rootItem() const { return m_items.container().at(0); }
	bool isUpdating() const { return m_updating; }
	void setUpdating(bool value) { m_updating = value; }

private:
	QString m_tag;
	bool m_updating;
	M3uContainer m_items;
	M3uProxyModel m_proxy;
	M3uDelegate m_delegate;
	INodeView::MenuActionList m_menuActions;
};

M3U_PLUGIN_NS_END

#endif /* M3UNODE_H_ */
