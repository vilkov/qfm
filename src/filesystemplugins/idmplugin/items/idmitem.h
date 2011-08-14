#ifndef IDMITEM_H_
#define IDMITEM_H_

#include "../../../filesystem/filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class IdmItem
{
public:
	IdmItem(IdmItem *parent) :
		m_parent(parent)
	{}

	IdmItem *parent() const { return m_parent; }

	virtual QVariant data(qint32 column, qint32 role) const = 0;
	virtual bool isRoot() const = 0;
	virtual bool isList() const = 0;
	virtual bool isMenuItem() const = 0;
	virtual bool isEntity() const = 0;

private:
	IdmItem *m_parent;
};

FILE_SYSTEM_NS_END

#endif /* IDMITEM_H_ */
