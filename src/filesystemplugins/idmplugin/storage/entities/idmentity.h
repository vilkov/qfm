#ifndef IDMENTITY_H_
#define IDMENTITY_H_

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include "../../idmplugin_ns.h"
#include "../../../../tools/containers/hashedlist.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntity
{
public:
	enum Type
	{
		Int        = 1,
		String     = 2,
		Date       = 3,
		Time       = 4,
		DateTime   = 5,
		Memo       = 6,
		Composite  = 7,
		Rating     = 8,
		Path       = 9
	};

	typedef unsigned int                    id_type;
	typedef HashedList<id_type, IdmEntity*> value_type;
	typedef value_type::size_type           size_type;
	enum { InvalidIndex = value_type::InvalidIndex };
	enum { InvalidId = (id_type)-1 };

	typedef QList<IdmEntity*> Parents;

public:
	IdmEntity(Type type, id_type id, const QString &name) :
		m_type(type),
		m_id(id),
		m_name(name)
	{}
	virtual ~IdmEntity()
	{}

	IdmEntity *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(IdmEntity *item) const { return m_items.indexOf(item); }
	size_type indexOf(id_type id) const { return m_items.indexOf(id); }

	Type type() const { return m_type; }
	id_type id() const { return m_id; }
	const QString &name() const { return m_name; }
	const Parents &parents() const { return m_parents; }

	void addParent(IdmEntity *parent) { m_parents.push_back(parent); }
	void removeParent(IdmEntity *parent) { m_parents.removeAt(m_parents.indexOf(parent)); }
	void removeParent(Parents::size_type index) { m_parents.removeAt(index); }

	void add(IdmEntity *item) { m_items.add(item->id(), item); }
	void remove(id_type id) { m_items.remove(id); }
	void remove(size_type index) { m_items.remove(index); }
	IdmEntity *take(size_type index) { return m_items.take(index); }

protected:
	value_type m_items;

private:
	Type m_type;
	id_type m_id;
	QString m_name;
	Parents m_parents;
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITY_H_ */
