#ifndef HASHEDLIST_H_
#define HASHEDLIST_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include "containers_ns.h"
#include "../metatemplates.h"


CONTAINERS_NS_BEGIN

template <typename Hash, typename T>
class HashedList
{
public:
	typedef QList<T>                      ValueList;
	typedef QList<Hash>                   KeyList;
	typedef typename ValueList::size_type size_type;
	typedef QMap<Hash, size_type>         ValueMap;
	enum { InvalidIndex = (size_type)-1 };
	enum IndexOfType
	{
		ByValue,
		ByHash
	};

public:
	HashedList()
	{}
	~HashedList()
	{}

	T &operator[](size_type index) { return m_list[index]; }
	const T &operator[](size_type index) const { return m_list[index]; }
	const T &at(size_type index) const { return m_list.at(index); }
	const T &last() const { return m_list.last(); }
	size_type lastIndex() const { return m_list.size() - 1; }
	T &last() { return m_list.last(); }

	bool isEmpty() const { return m_list.isEmpty(); }
	size_type size() const { return m_list.size(); }
	size_type indexOf(const Hash &hash) const { return m_map.value(hash, InvalidIndex); }
	bool contains(const Hash &hash) const { return m_map.contains(hash); }

	KeyList keys() const { return m_map.keys(); }
	const ValueList &values() const { return m_list; }

	void add(const Hash &hash, const T &value)
	{
		m_list.push_back(value);
		m_map[hash] = m_list.size() - 1;
	}
	void remove(const Hash &hash)
	{
		size_type index = indexOf(hash);

		if (index != InvalidIndex)
		{
			m_map.remove(hash);
			m_list.removeAt(index);
		}
	}
	void remove(size_type index)
	{
		const T &value = m_list.at(index);

		for (typename ValueMap::iterator it = m_map.begin(), end = m_map.end(); it != end; ++it)
			if (*it == index)
			{
				for (it = m_map.erase(it); it != end; ++it)
					if (*it > index)
						--*it;

				break;
			}
			else
				if (*it > index)
					--*it;

		m_list.removeAt(index);
	}
	T take(const Hash &hash)
	{
		size_type index = indexOf(hash);

		if (index != InvalidIndex)
		{
			m_map.remove(hash);
			return m_list.takeAt(index);
		}

		return T();
	}
	T take(size_type index)
	{
		T res = m_list.at(index);
		remove(index);
		return res;
	}
	void clear()
	{
		m_list.clear();
		m_map.clear();
	}

private:
	ValueList m_list;
	ValueMap m_map;
};

CONTAINERS_NS_END

#endif /* HASHEDLIST_H_ */
