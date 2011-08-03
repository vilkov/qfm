#ifndef IDMENTITYVALUE_H_
#define IDMENTITYVALUE_H_

#include "idmentity.h"


FILE_SYSTEM_NS_BEGIN

class IdmEntityValue : public IdmEntity
{
public:
	IdmEntityValue(Type type, id_type id, const QString &name) :
		IdmEntity(id, name)
	{}

	/* IdmEntity */
	virtual Type type() const { return m_type; }

private:
	Type m_type;
};

FILE_SYSTEM_NS_END

#endif /* IDMENTITYVALUE_H_ */
