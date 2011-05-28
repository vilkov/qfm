#ifndef FILESYSTEMUPDATESLIST_H_
#define FILESYSTEMUPDATESLIST_H_

#include <QtCore/QSet>
#include <QtCore/QList>
#include "../items/filesystemfoldernodeitem.h"
#include "../containers/filesystemfoldernodevalues.h"


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
		Change(Type type = NoChange) :
			m_type(type)
		{}
		Change(Type type, Values::size_type index) :
			m_type(type),
			m_index(index)
		{}
		Change(Type type, const Info &info, Values::size_type index = Values::InvalidIndex) :
			m_type(type),
			m_info(info),
			m_index(index)
		{}

		Type type() const { return m_type; }
		const Info &info() const { return m_info; }
		Values::size_type index() const { return m_index; }

	private:
		Type m_type;
		Info m_info;
		Values::size_type m_index;
	};

	typedef QList<Change>           List;
	typedef List::size_type         size_type;
	typedef QSet<Values::size_type> Set;

public:
	UpdatesList(const Values &values) :
		m_values(values)
	{
		m_changes.reserve(m_values.size());
	}

	void operator=(const UpdatesList &other) { m_changes = other.m_changes; }

	const Change &operator[](size_type index) const { return m_changes[index]; }
	const Change &at(size_type index) const { return m_changes.at(index); }

	bool isEmpty() const { return m_changes.isEmpty(); }
	size_type size() const { return m_changes.size(); }

	void removeAt(size_type index) { m_changes.removeAt(index); }

#ifndef Q_OS_WIN
	Values::size_type update(const QFileInfo &info, uint userId, uint groupId)
	{
		Values::size_type index = m_values.indexOf(info.fileName());

		if (index == Values::InvalidIndex)
			m_changes.push_back(Change(Added, Info(info, userId, groupId)));
		else
			if (m_values.at(index).item->lastModified() != info.lastModified())
				m_changes.push_back(Change(Updated, Info(info, userId, groupId), index));

		return index;
	}
#else
	Values::size_type update(const QFileInfo &info)
	{
		Values::size_type index = m_values.indexOf(info.fileName());

		if (index == Values::InvalidIndex)
			m_changes.push_back(Change(Added, info));
		else
			if (m_values.at(index).item->lastModified() != info.lastModified())
				m_changes.push_back(Change(Updated, info, index));

		return index;
	}
#endif
	void update(const Set &affected)
	{
		Set res = affected;

		res.remove(Values::InvalidIndex);
		res = m_values.indexes().subtract(res);

		if (m_values.at(0).item->isRootItem())
			res.remove(0);

		for (Set::const_iterator it = res.constBegin(), end = res.constEnd(); it != end; ++it)
			m_changes.push_back(Change(Deleted, *it));
	}

private:
	List m_changes;
	const Values &m_values;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMUPDATESLIST_H_ */
