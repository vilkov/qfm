#ifndef ARCENTRYNODEITEM_H_
#define ARCENTRYNODEITEM_H_

#include <vfs/filetypeinfo/vfs_filetypeinfo.h>
#include <vfs/containers/vfs_wrappednodeitem.h>
#include "arcnodeitem.h"


ARC_PLUGIN_NS_BEGIN

class EntryNodeItem : public NodeItem
{
public:
	typedef QList<Holder> Container;

public:
	EntryNodeItem(WrappedNodeItem *item, Base *parent = 0);

	/* Base */
	virtual Base *at(size_type index) const;
	virtual size_type size() const;
	virtual size_type indexOf(Base *item) const;
	virtual QVariant data(qint32 column, qint32 role) const;

	/* ArcNodeItem */
	virtual void lock(const QString &reason);

	const IFileInfo::Holder &info() const { return m_info; }

private:
	Container m_items;
	IFileInfo::Holder m_info;
	IFileInfo::size_type m_totalSize;
};

ARC_PLUGIN_NS_END

#endif /* ARCENTRYNODEITEM_H_ */
