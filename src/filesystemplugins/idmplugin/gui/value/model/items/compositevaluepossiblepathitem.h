#ifndef COMPOSITEVALUEPOSSIBLEPATHITEM_H_
#define COMPOSITEVALUEPOSSIBLEPATHITEM_H_

#include "compositevaluepathitem.h"
#include "../../../../../../filesystem/tasks/concrete/containers/filesysteminfoitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValuePossiblePathItem : public CompositeValuePathItem
{
public:
	CompositeValuePossiblePathItem(const IdmEntityValue::Holder &value, const InfoItem *source, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* CompositeValuePathItem */
	virtual void open() const;

protected:
	const InfoItem *m_source;
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEPOSSIBLEPATHITEM_H_ */
