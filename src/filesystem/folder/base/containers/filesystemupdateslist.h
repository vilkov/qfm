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
	typedef QSet<QString>         Locked;
	typedef QMap<QString, Change> Changes;
	typedef Changes::iterator     iterator;
	typedef QList<Change>         Values;

public:
	UpdatesList()
	{}
	UpdatesList(const Changes &changes) :
		m_changes(changes)
	{}
	UpdatesList(const Locked &locked, const Changes &changes) :
		m_locked(locked),
		m_changes(changes)
	{}

	bool isEmpty() const { return m_changes.isEmpty(); }
	bool isLocked(const QString &fileName) const { return m_locked.contains(fileName); }

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
			if (change.info().shouldBeUpdatedBy(info))
				change.update(info, Updated);
			else
				change.update(NoChange);
	}
	UpdatesList takeUpdates()
	{
		Changes changes;

		for (Changes::iterator it = m_changes.begin(), end = m_changes.end(); it != end;)
			if (it.value().type() == Deleted)
				++it;
			else
			{
				if (it.value().type() != NoChange)
					changes.insert(it.key(), it.value());

				it = m_changes.erase(it);
			}

		return UpdatesList(changes);
	}

private:
	Locked m_locked;
	Changes m_changes;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMUPDATESLIST_H_ */
