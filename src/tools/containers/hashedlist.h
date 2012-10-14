/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef HASHEDLIST_H_
#define HASHEDLIST_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include "containers_ns.h"
#include "../templates/metatemplates.h"


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


	class ByIndex
	{
	public:
		typedef HashedList<Hash, T> Container;

	public:
		ByIndex(Container &container) :
			m_container(container)
		{}

		T &operator[](size_type index) { return m_container.m_list[index]; }
		const T &operator[](size_type index) const { return m_container.m_list[index]; }

		const T &at(size_type index) const { return m_container.m_list.at(index); }

		T &last() { return m_container.m_list.last(); }
		const T &last() const { return m_container.m_list.last(); }

		size_type lastIndex() const { return m_container.m_list.size() - 1; }

		bool isEmpty() const { return m_container.m_list.isEmpty(); }
		size_type size() const { return m_container.m_list.size(); }
		size_type indexOf(const T &value) const { return m_container.m_list.indexOf(value); }
		bool contains(const T &value) const { return m_container.m_list.contains(value); }

	private:
		Container &m_container;
	};
	friend class ByIndex;


//	class ByHash
//	{
//	public:
//		typedef HashedList<Hash, T> Container;
//
//	public:
//		ByHash(Container &container) :
//			m_emptyValue(),
//			m_container(container)
//		{}
//
//		T &operator[](const Hash &hash)
//		{
//			size_type index;
//
//			if ((index = m_container.m_map.value(hash, InvalidIndex)) == InvalidIndex)
//			{
//				m_map.insert(hash, m_container.m_list.size());
//				m_container.m_list.push_back(T());
//
//				return m_container.m_list.last();
//			}
//
//			return m_container.m_list[index];
//		}
//		const T &operator[](size_type index) const { return m_container.m_list[index]; }
//
//		const T &at(size_type index) const { return m_container.m_list.at(index); }
//
//		T &last() { return m_container.m_list.last(); }
//		const T &last() const { return m_container.m_list.last(); }
//
//		size_type lastIndex() const { return m_container.m_list.size() - 1; }
//
//		bool isEmpty() const { return m_container.m_list.isEmpty(); }
//		size_type size() const { return m_container.m_list.size(); }
//		size_type indexOf(const T &value) const { return m_container.m_list.indexOf(value); }
//		bool contains(const T &value) const { return m_container.m_list.contains(value); }
//
//	private:
//		T m_emptyValue;
//		Container &m_container;
//	};
//	friend class ByHash;

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
	size_type indexOf(const T &value) const { return m_list.indexOf(value); }
	bool contains(const Hash &hash) const { return m_map.contains(hash); }

	KeyList keys() const { return m_map.keys(); }
	const ValueList &values() const { return m_list; }

	QSet<T> toSet() const { return m_list.toSet(); }

	void add(const Hash &hash, const T &value)
	{
		m_list.push_back(value);
		m_map[hash] = m_list.size() - 1;
	}
	void remove(const Hash &hash)
	{
		size_type index = indexOf(hash);

		if (index != InvalidIndex)
			remove(index);
	}
	void remove(size_type index)
	{
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
			return take(index);

		return T();
	}
	void replace(size_type index, const Hash &oldHash, const Hash &newHash, const T &value)
	{
		m_list[index] = value;
		m_map[newHash] = index;
		m_map.remove(oldHash);
	}
	void replace(size_type index, const Hash &oldHash, const Hash &newHash)
	{
		m_map[newHash] = index;
		m_map.remove(oldHash);
	}
	void replace(const Hash &oldHash, const Hash &newHash)
	{
		m_map[newHash] = m_map.take(oldHash);
	}
	T take(size_type index)
	{
		T res(m_list.at(index));
		remove(index);
		return res;
	}
	void clear()
	{
		m_list.clear();
		m_map.clear();
	}
	void deleteAll()
	{
		qDeleteAll(m_list);
	}

private:
	ValueList m_list;
	ValueMap m_map;
};

CONTAINERS_NS_END

#endif /* HASHEDLIST_H_ */
