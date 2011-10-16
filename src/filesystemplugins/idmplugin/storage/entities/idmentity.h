#ifndef IDMENTITY_H_
#define IDMENTITY_H_

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include "../format/idmshortformat.h"
#include "../structure/idmdatabasetypes.h"
#include "../../../../tools/containers/hashedlist.h"


IDM_PLUGIN_NS_BEGIN

class IdmEntity
{
public:
	struct Property
	{
		Property()
		{}
		Property(IdmEntity *entity, const QString &name) :
			entity(entity),
			name(name)
		{}

		bool operator==(const Property &other) const { return entity == other.entity; }

		IdmEntity *entity;
		QString name;
	};
	typedef Database::EntityType                                 Type;
	typedef Database::id_type                                    id_type;
	typedef ::Tools::Containers::HashedList<id_type, Property>   Container;
	typedef Container::size_type                                 size_type;
	typedef ::Tools::Containers::HashedList<id_type, IdmEntity*> Parents;
	enum { InvalidIndex = Container::InvalidIndex };
	enum { InvalidId = Database::InvalidId };

public:
	IdmEntity(Type type, id_type id, const QString &name, const IdmShortFormat &shortFormat) :
		m_type(type),
		m_id(id),
		m_name(name),
		m_shortFormat(shortFormat)
	{}
	virtual ~IdmEntity()
	{}

	const Property &at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(id_type id) const { return m_items.indexOf(id); }
	size_type indexOf(IdmEntity *entity) const
	{
		for (size_type i = 0, size = m_items.size(); i < size; ++i)
			if (m_items.at(i).entity == entity)
				return i;

		return InvalidIndex;
	}
	size_type indexOf(const QString &name) const
	{
		for (size_type i = 0, size = m_items.size(); i < size; ++i)
			if (m_items.at(i).name.compare(name, Qt::CaseSensitive) == 0)
				return i;

		return InvalidIndex;
	}

	Type type() const { return m_type; }
	id_type id() const { return m_id; }
	const QString &name() const { return m_name; }
	const Parents &parents() const { return m_parents; }
	const IdmShortFormat &shortFormat() const { return m_shortFormat; }

	void addParent(IdmEntity *parent) { m_parents.add(parent->id(), parent); }
	void removeParent(IdmEntity *parent) { m_parents.remove(parent->id()); }

	void add(IdmEntity *item, const QString &name) { m_items.add(item->id(), Property(item, name)); }
	void remove(IdmEntity *item) { m_items.remove(item->id()); }
	IdmEntity *take(size_type index) { return m_items.take(index).entity; }

protected:
	Container m_items;

private:
	Type m_type;
	id_type m_id;
	QString m_name;
	Parents m_parents;
	IdmShortFormat m_shortFormat;
};

IDM_PLUGIN_NS_END

#endif /* IDMENTITY_H_ */
