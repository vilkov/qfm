#ifndef IDMSEPARATOR_H_
#define IDMSEPARATOR_H_

#include "idmitem.h"


FILE_SYSTEM_NS_BEGIN

class IdmSeparator : public IdmItem
{
public:
	IdmSeparator(IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;
	virtual bool isRoot() const;
	virtual bool isList() const;
	virtual bool isMenuItem() const;
	virtual bool isEntityItem() const;
};

FILE_SYSTEM_NS_END

#endif /* IDMSEPARATOR_H_ */
