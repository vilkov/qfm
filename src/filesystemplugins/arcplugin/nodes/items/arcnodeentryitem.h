#ifndef ARCNODEENTRYITEM_H_
#define ARCNODEENTRYITEM_H_

#include "arcnodeitem.h"
#include "../../../../filesystem/filetypeinfo/filetypeinfo.h"
#include "../../../../filesystem/containers/filesystemwrappednodeitem.h"


ARC_PLUGIN_NS_BEGIN

class ArcNodeEntryItem : public ArcNodeItem
{
public:
	typedef QList<ArcNodeEntryItem::Holder> Container;

public:
	ArcNodeEntryItem(WrappedNodeItem *item, Base *parent = 0);

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

#endif /* ARCNODEENTRYITEM_H_ */
