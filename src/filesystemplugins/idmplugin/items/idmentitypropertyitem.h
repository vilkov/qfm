#ifndef IDMENTITYPROPERTYITEM_H_
#define IDMENTITYPROPERTYITEM_H_

#include "idmentityitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntityPropertyItem : public IdmEntityItem
{
public:
	IdmEntityPropertyItem(IdmEntity *entity, const QString &name, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	const QString &name() const { return m_name; }

protected:
	QString m_name;
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITYPROPERTYITEM_H_ */
