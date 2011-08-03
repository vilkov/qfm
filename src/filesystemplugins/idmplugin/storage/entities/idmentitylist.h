#ifndef IDMENTITYLIST_H_
#define IDMENTITYLIST_H_

#include <QtCore/QList>
#include "idmentity.h"
#include "../../../../tools/containers/hashedlist.h"


FILE_SYSTEM_NS_BEGIN

class IdmEntityList : public IdmEntity
{
public:
	typedef HashedList<id_type, IdmEntity*> value_type;
	typedef value_type::size_type           size_type;
	enum { InvalidIndex = value_type::InvalidIndex };

public:
	IdmEntityList(id_type id, const QString &name) :
		IdmEntity(id, name)
	{}

	/* IdmEntity */
	virtual Type type() const { return Composite; }

	IdmEntity *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(IdmEntity *item) const { return m_items.indexOf(item); }
	size_type indexOf(id_type id) const { return m_items.indexOf(id); }

	void add(IdmEntity *item) { m_items.add(item->id(), item); }
	void remove(id_type id) { m_items.remove(id); }
	void remove(size_type index) { m_items.remove(index); }
	IdmEntity *take(size_type index) { return m_items.take(index); }

protected:
	value_type m_items;
};

FILE_SYSTEM_NS_END

#endif /* IDMENTITYLIST_H_ */
