#ifndef IDMENTITYVALUEITEM_H_
#define IDMENTITYVALUEITEM_H_

#include "idmentityitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntityValueItem : public IdmEntityItem
{
public:
	IdmEntityValueItem(IdmEntity *entity, const QVariant &value, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

protected:
	QVariant m_value;
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITYVALUEITEM_H_ */
