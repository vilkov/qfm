#ifndef DEFAULTSEARCHNODEITEM_H_
#define DEFAULTSEARCHNODEITEM_H_

#include "../../../model/items/defaultnodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN

class SearchNodeItem : public NodeItem
{
public:
	typedef QList<Holder> Container;

public:
	SearchNodeItem(SnapshotItem::Holder &file, Base *parent = 0);

	/* ::Tools::Models::Tree::Item */
	virtual QVariant data(qint32 column, qint32 role) const;

	const IFileContainer *container() const { return m_file->container(); }

private:
	SnapshotItem::Holder m_file;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTSEARCHNODEITEM_H_ */
