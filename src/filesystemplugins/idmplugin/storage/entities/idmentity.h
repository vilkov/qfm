#ifndef IDMENTITY_H_
#define IDMENTITY_H_

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include "../../items/idmitemslist.h"


FILE_SYSTEM_NS_BEGIN

class IdmEntity : public IdmItemsList
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
		Rating     = 8
	};

	typedef unsigned int id_type;
	enum { InvalidId = (id_type)-1 };

	typedef QList<IdmEntity*> Parents;

public:
	IdmEntity(Type type, id_type id, const QString &name) :
		IdmItemsList(0),
		m_type(type),
		m_id(id),
		m_name(name)
	{}

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const
	{
		if (column == 0 && role == Qt::DisplayRole)
			return name();
		else
			return QVariant();
	}
	virtual bool isEntity() const { return true; }

	IdmEntity *at(size_type index) const { return static_cast<IdmEntity *>(m_items.at(index)); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(IdmEntity *item) const { return m_items.indexOf(item); }
	size_type indexOf(id_type id) const
	{
		for (size_type i = 0, sz = size(); i < sz; ++i)
			if (at(i)->id() == id)
				return i;

		return InvalidIndex;
	}

	Type type() const { return m_type; }
	id_type id() const { return m_id; }
	const QString &name() const { return m_name; }
	const Parents &parents() const { return m_parents; }

	void addParent(IdmEntity *parent) { m_parents.push_back(parent); }
	void removeParent(Parents::size_type index) { m_parents.removeAt(index); }

	void add(IdmEntity *item) { m_items.push_back(item); }
	void remove(id_type id) { m_items.removeAt(indexOf(id)); }
	void remove(size_type index) { m_items.removeAt(index); }
	IdmEntity *take(size_type index) { return static_cast<IdmEntity *>(m_items.takeAt(index)); }

private:
	Type m_type;
	id_type m_id;
	QString m_name;
	Parents m_parents;
};

FILE_SYSTEM_NS_END

#endif /* IDMENTITY_H_ */
