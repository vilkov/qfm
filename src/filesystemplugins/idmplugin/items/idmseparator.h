#ifndef IDMSEPARATOR_H_
#define IDMSEPARATOR_H_

#include "idmbaseitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmSeparator : public IdmBaseItem
{
public:
	IdmSeparator(IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* IBaseItem */
	virtual bool isRoot() const;
	virtual bool isList() const;
	virtual bool isMenuItem() const;
	virtual bool isValueItem() const;
	virtual bool isEntityItem() const;
};

IDM_PLUGIN_NS_END

#endif /* IDMSEPARATOR_H_ */
