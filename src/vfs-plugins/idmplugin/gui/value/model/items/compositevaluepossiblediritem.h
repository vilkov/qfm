#ifndef COMPOSITEVALUEPOSSIBLEDIRITEM_H_
#define COMPOSITEVALUEPOSSIBLEDIRITEM_H_

#include <vfs/containers/vfs_wrappednodeitem.h>
#include "compositevaluepathitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValuePossibleDirItem : public CompositeValuePathItem
{
public:
	typedef QList<IdmItem *> Container;

public:
	CompositeValuePossibleDirItem(const IdmEntityValue::Holder &value, const WrappedNodeItem *source, IdmItem *parent = 0);
	virtual ~CompositeValuePossibleDirItem();

	/* Base */
	virtual Base *at(size_type index) const;
	virtual size_type size() const;
	virtual size_type indexOf(Base *item) const;

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* CompositeValuePathItem */
	virtual QString fileName() const;
	virtual bool isFile() const;
	virtual void open() const;

protected:
	friend class CompositeValueModel;
	void add(IdmItem *item) { m_items.push_back(item); }
	void remove(size_type index) { delete m_items.takeAt(index); }
	const WrappedNodeItem *source() const { return m_source; }

private:
	const WrappedNodeItem *m_source;
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEPOSSIBLEDIRITEM_H_ */
