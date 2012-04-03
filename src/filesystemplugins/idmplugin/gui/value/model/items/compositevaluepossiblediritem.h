#ifndef COMPOSITEVALUEPOSSIBLEDIRITEM_H_
#define COMPOSITEVALUEPOSSIBLEDIRITEM_H_

#include "compositevaluepathitem.h"
#include "../../../../../../filesystem/tasks/concrete/containers/filesysteminfoitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValuePossibleDirItem : public CompositeValuePathItem
{
public:
	typedef QList<IdmItem *> Container;

public:
	CompositeValuePossibleDirItem(const IdmEntityValue::Holder &value, const InfoItem *source, IdmItem *parent = 0);
	virtual ~CompositeValuePossibleDirItem();

	/* Base */
	virtual Base *at(size_type index) const;
	virtual size_type size() const;
	virtual size_type indexOf(Base *item) const;

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* CompositeValuePathItem */
	virtual void open() const;

protected:
	friend class CompositeValueModel;
	void add(IdmItem *item) { m_items.push_back(item); }
	void remove(size_type index) { delete m_items.takeAt(index); }
	const InfoItem *source() const { return m_source; }

private:
	const InfoItem *m_source;
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEPOSSIBLEDIRITEM_H_ */
