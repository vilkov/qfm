#ifndef COMPOSITEVALUEPOSSIBLEPATHITEM_H_
#define COMPOSITEVALUEPOSSIBLEPATHITEM_H_

#include "compositevalueitem.h"
#include "../../../../../../filesystem/tasks/concrete/containers/filesysteminfoitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValuePossiblePathItem : public CompositeValueItem
{
public:
	CompositeValuePossiblePathItem(const IdmEntityValue::Holder &value, const InfoItem *source, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* CompositeValueItem */
	virtual bool isPath() const;

	const IdmEntityValue::Holder &value() const { return m_value; }
	IdmEntityValue::Holder take() { IdmEntityValue::Holder res(m_value); m_value.reset(); return res; }

protected:
	IdmEntityValue::Holder m_value;
	const InfoItem *m_source;
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEPOSSIBLEPATHITEM_H_ */
