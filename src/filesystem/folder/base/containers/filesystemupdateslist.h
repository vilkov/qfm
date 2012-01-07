#ifndef FILESYSTEMUPDATESLIST_H_
#define FILESYSTEMUPDATESLIST_H_

#include <QtCore/QSet>
#include <QtCore/QMap>
#include <QtCore/QList>
#include "../../../info/filesysteminfo.h"


FILE_SYSTEM_NS_BEGIN

class UpdatesList
{
public:
	enum Type
	{
		Added,
		Deleted,
		Updated,
		NoChange
	};

	class Change
	{
	public:
		Change() :
			m_type(Added)
		{}
		Change(const Info &info) :
			m_info(info),
			m_type(Deleted)
		{}

		Type type() const { return m_type; }
		const Info &info() const { return m_info; }
		void update(Type type) { m_type = type; }
		void update(const Info &info) { m_info = info; }
		void update(const Info &info, Type type) { m_info = info; m_type = type; }

	private:
		Info m_info;
		Type m_type;
	};
	typedef QMap<QString, Change> Map;
	typedef Map::iterator         iterator;
	typedef QList<Change>         Values;

public:
	UpdatesList()
	{}
	UpdatesList(const Map &changes) :
		m_changes(changes)
	{}

	bool isEmpty() const { return m_changes.isEmpty(); }

	iterator begin() { return m_changes.begin(); }
	iterator end() { return m_changes.end(); }
	iterator erase(iterator it) { return m_changes.erase(it); }
	Values values() const { return m_changes.values(); }

	void update(const Info &info)
	{
		Change &change = m_changes[info.fileName()];

		if (change.type() == Added)
			change.update(info);
		else
			if (change.type() == Deleted)
				if (change.info().shouldBeUpdatedBy(info))
					change.update(info, Updated);
				else
					change.update(NoChange);
	}
	UpdatesList takeUpdates()
	{
		Map changes;

		for (Map::iterator it = m_changes.begin(), end = m_changes.end(); it != end;)
			if (it.value().type() == NoChange)
				it = m_changes.erase(it);
			else
				if (it.value().type() != Deleted)
				{
					changes.insert(it.key(), it.value());
					it = m_changes.erase(it);
				}
				else
					++it;

		return UpdatesList(changes);
	}

private:
	Map m_changes;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMUPDATESLIST_H_ */
