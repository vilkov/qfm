#ifndef IDM_COMPOSITEVALUEPOSSIBLEFILEITEM_H_
#define IDM_COMPOSITEVALUEPOSSIBLEFILEITEM_H_

#include <vfs/containers/vfs_snapshotitem.h>
#include "idm_compositevaluepathitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValuePossibleFileItem : public CompositeValuePathItem
{
public:
	CompositeValuePossibleFileItem(const EntityValue::Holder &value, const SnapshotItem *source, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* CompositeValuePathItem */
	virtual QString fileName() const;
	virtual bool isFile() const;
	virtual void open() const;

protected:
	const SnapshotItem *source() const { return m_source; }

private:
	const SnapshotItem *m_source;
};

IDM_PLUGIN_NS_END

#endif /* IDM_COMPOSITEVALUEPOSSIBLEFILEITEM_H_ */
