#ifndef IDMENTITYVALUEITEM_H_
#define IDMENTITYVALUEITEM_H_

#include "idmentityitem.h"
#include "../storage/values/idmentityvalue.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntityValueItem : public IdmEntityItem
{
public:
	IdmEntityValueItem(IdmEntity *entity, IdmEntityValue *value, IdmItem *parent = 0);
	virtual ~IdmEntityValueItem();

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;
	virtual bool isValueItem() const;

protected:
	IdmEntityValue *m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITYVALUEITEM_H_ */
