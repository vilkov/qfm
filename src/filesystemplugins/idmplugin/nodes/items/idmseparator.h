#ifndef IDMSEPARATOR_H_
#define IDMSEPARATOR_H_

#include "idmitem.h"


FILE_SYSTEM_NS_BEGIN

class IdmSeparator : public IdmItem
{
public:
	IdmSeparator()
	{}

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const { return QVariant(); }
	virtual bool isRoot() const { return false; }
};

FILE_SYSTEM_NS_END

#endif /* IDMSEPARATOR_H_ */
