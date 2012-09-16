#ifndef IDM_COMPOSITEVALUEPOSSIBLEDIRITEM_H_
#define IDM_COMPOSITEVALUEPOSSIBLEDIRITEM_H_

#include <vfs/containers/vfs_snapshotitem.h>
#include "idm_compositevaluepathitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValuePossibleDirItem : public CompositeValuePathItem
{
public:
	typedef QList<IdmItem *> Container;

public:
	CompositeValuePossibleDirItem(const IdmEntityValue::Holder &value, const SnapshotItem *source, IdmItem *parent = 0);
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
	const SnapshotItem *source() const { return m_source; }

private:
	const SnapshotItem *m_source;
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEPOSSIBLEDIRITEM_H_ */
