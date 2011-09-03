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

	struct Property
	{
		Property()
		{}
		Property(IdmEntity *entity) :
			entity(entity)
		{}
		Property(IdmEntity *entity, const QString &name) :
			entity(entity),
			name(name)
		{}

		bool operator==(const Property &other) const { return entity == other.entity; }

		IdmEntity *entity;
		QString name;
	};
	typedef unsigned int                  id_type;
	typedef HashedList<id_type, Property> value_type;
	typedef value_type::size_type         size_type;
	enum { InvalidIndex = value_type::InvalidIndex };
	enum { InvalidId = (id_type)-1 };

	typedef HashedList<id_type, IdmEntity*> Parents;

public:
	IdmEntity(Type type, id_type id, const QString &name) :
		m_type(type),
		m_id(id),
		m_name(name)
	{}
	virtual ~IdmEntity()
	{}

	IdmEntity *at(size_type index) const { return m_items.at(index).entity; }
	size_type size() const { return m_items.size(); }
	size_type indexOf(IdmEntity *item) const { return m_items.indexOf(item); }
	size_type indexOf(id_type id) const { return m_items.indexOf(id); }
	const Property &property(size_type index) const { return m_items.at(index); }
	const Property &property(IdmEntity *item) const { return m_items.at(item->id()); }

	Type type() const { return m_type; }
	id_type id() const { return m_id; }
	const QString &name() const { return m_name; }
	const Parents &parents() const { return m_parents; }

	void addParent(IdmEntity *parent) { m_parents.add(parent->id(), parent); }
	void removeParent(IdmEntity *parent) { m_parents.remove(parent->id()); }

	void add(IdmEntity *item, const QString &name) { m_items.add(item->id(), Property(item, name)); }
	void remove(IdmEntity *item) { m_items.remove(item->id()); }
	IdmEntity *take(size_type index) { return m_items.take(index).entity; }

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
