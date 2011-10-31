#ifndef IDMNODEBASE_H_
#define IDMNODEBASE_H_

#include "idmnodedelegate.h"
#include "idmnodeproxymodel.h"
#include "../idmplugin_ns.h"
#include "../items/idmbaseitem.h"
#include "../functors/idmfunctors.h"
#include "../model/idmmodelcontainer.h"
#include "../containeres/idmcontainer.h"
#include "../../../filesystem/folder/base/filesystemfoldernodebase.h"


IDM_PLUGIN_NS_BEGIN

class IdmNodeBase : public FolderNodeBase
{
public:
	IdmNodeBase(const IdmContainer &container, const Info &info, Node *parent = 0);
	virtual ~IdmNodeBase();

    /* FileSystemModel */
	virtual int columnCount(const QModelIndex &parent) const;

	/* INode */
	virtual IFileInfo *info(const QModelIndex &idx) const;
	virtual ICopyControl *createControl() const;

	/* INode::IFileOperations */
	virtual void menuAction(QAction *action, INodeView *view);
	virtual void createFile(const QModelIndex &index);
	virtual void createDirectory(const QModelIndex &index);
	virtual void rename(const QModelIndexList &list);
	virtual void remove(const QModelIndexList &list);
	virtual void cancel(const QModelIndexList &list);
	virtual void calculateSize(const QModelIndexList &list);
	virtual void pathToClipboard(const QModelIndexList &list);
	virtual void copy(const QModelIndexList &list, INode *destination);
	virtual void move(const QModelIndexList &list, INode *destination);

protected:
	/* Node */
	virtual QModelIndex rootIndex() const;
	virtual QAbstractItemModel *proxyModel() const { return &((IdmNodeBase *)this)->m_proxy; }
	virtual QAbstractItemDelegate *itemDelegate() const { return &((IdmNodeBase *)this)->m_delegate; }
	virtual const INodeView::MenuActionList &menuActions() const { return m_container.menuActions(); }

	virtual Node *viewChild(const QModelIndex &idx, PluginsManager *plugins, QModelIndex &selected);
	virtual Node *viewChild(const QString &fileName, PluginsManager *plugins, QModelIndex &selected);

protected:
	/* FolderNodeBase */
	virtual UpdatesList::Map updateFilesMap() const;
	virtual void updateFilesEvent(const UpdatesList &updates);
	virtual void scanForSizeEvent(bool canceled, const InfoListItem *entries);
	virtual bool scanForCopyEvent(bool canceled, const InfoListItem *entries, ICopyControl *control, bool move);
	virtual bool scanForRemoveEvent(bool canceled, const InfoListItem *entries);
	virtual bool performCopyEvent(bool canceled, const InfoListItem *entries, bool move);
	virtual void performRemoveEvent(const InfoListItem *entries);

	virtual void updateProgressEvent(const QString &fileName, quint64 progress, quint64 timeElapsed);
	virtual void completedProgressEvent(const QString &fileName, quint64 timeElapsed);

protected:
	IdmItem *rootItem() const { return m_items.at(1); }
	QModelIndex index(IdmItem *item) const;

	bool processIndexList(const QModelIndexList &list, const IdmFunctors::Functor &functor) const;
	bool processRemoveItem(const QModelIndex &index, IdmItem *item);

protected:
	IdmModelContainer m_itemsContainer;
	IdmModelContainer::Container &m_items;
	IdmNodeProxyModel m_proxy;
	IdmNodeDelegate m_delegate;
	IdmContainer m_container;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEBASE_H_ */
