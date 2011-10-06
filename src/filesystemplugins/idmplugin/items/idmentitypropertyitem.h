#ifndef IDMENTITYPROPERTYITEM_H_
#define IDMENTITYPROPERTYITEM_H_

#include "idmentityitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntityPropertyItem : public IdmEntityItem
{
public:
	IdmEntityPropertyItem(const QString &name, IdmEntity *entity, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	const QVariant &name() const { return m_name; }

protected:
	QVariant m_name;
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITYPROPERTYITEM_H_ */
