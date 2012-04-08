#ifndef COMPOSITEVALUEPOSSIBLEFILEITEM_H_
#define COMPOSITEVALUEPOSSIBLEFILEITEM_H_

#include "compositevaluepathitem.h"
#include "../../../../../../filesystem/tasks/concrete/containers/filesysteminfoitem.h"


IDM_PLUGIN_NS_BEGIN

class CompositeValuePossibleFileItem : public CompositeValuePathItem
{
public:
	CompositeValuePossibleFileItem(const IdmEntityValue::Holder &value, const InfoItem *source, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* CompositeValuePathItem */
	virtual QString fileName() const;
	virtual bool isFile() const;
	virtual void open() const;

protected:
	const InfoItem *source() const { return m_source; }

private:
	const InfoItem *m_source;
};

IDM_PLUGIN_NS_END

#endif /* COMPOSITEVALUEPOSSIBLEFILEITEM_H_ */
