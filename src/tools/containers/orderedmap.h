#ifndef ORDEREDMAP_H_
#define ORDEREDMAP_H_

#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QLinkedList>
#include "containers_ns.h"


CONTAINERS_NS_BEGIN

template <typename Key, typename T>
class OrderedMap
{
public:
	typedef QPair<Key, T>                      Pair;
	typedef QLinkedList<Pair>                  List;
	typedef QMap<Key, typename List::iterator> Map;

    class const_iterator;
    class iterator
    {
    public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef T value_type;
        typedef T *pointer;
        typedef T &reference;
        friend class const_iterator;

    public:
        inline iterator() {}
        inline iterator(const iterator &o) : m_it(o.m_it) {}
        inline iterator(const typename List::iterator &o) : m_it(o) {}
        inline iterator &operator=(const iterator &o) { m_it = o.m_it; return *this; }
        inline const Key &key() const { return m_it->first; }
        inline T &value() const { return m_it->second; }
        inline T &operator*() const { return m_it->second; }
        inline T *operator->() const { return &m_it->second; }
        inline bool operator==(const iterator &o) const { return m_it == o.m_it; }
        inline bool operator!=(const iterator &o) const { return m_it != o.m_it; }
        inline bool operator==(const const_iterator &o) const { return m_it == o.m_it; }
        inline bool operator!=(const const_iterator &o) const { return m_it != o.m_it; }
        inline iterator &operator++() { ++m_it; return *this; }
        inline iterator operator++(int) { return (m_it++); }
        inline iterator &operator--() { --m_it; return *this; }
        inline iterator operator--(int) { return (m_it--); }
        inline iterator operator+(int j) const { return (m_it + j); }
        inline iterator operator-(int j) const { return operator+(-j); }
        inline iterator &operator+=(int j) { return *this = *this + j; }
        inline iterator &operator-=(int j) { return *this = *this - j; }

    private:
        typename List::iterator m_it;
    };

    class const_iterator
    {
    public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef qptrdiff difference_type;
        typedef T value_type;
        typedef const T *pointer;
        typedef const T &reference;
        friend class iterator;

    public:
        inline const_iterator() {}
        inline const_iterator(const const_iterator &o) : m_it(o.m_it) {}
        inline const_iterator(iterator ci) : m_it(ci.m_it) {}
        inline const_iterator(const typename List::const_iterator &o) : m_it(o) {}
        inline const_iterator &operator=(const const_iterator &o) { m_it = o.m_it; return *this; }
        inline const Key &key() const { return m_it->first; }
        inline T &value() const { return m_it->second; }
        inline const T &operator*() const { return m_it->second; }
        inline const T *operator->() const { return &m_it->second; }
        inline bool operator==(const const_iterator &o) const { return m_it == o.m_it; }
        inline bool operator!=(const const_iterator &o) const { return m_it != o.m_it; }
        inline const_iterator &operator++() { ++m_it; return *this; }
        inline const_iterator operator++(int) { return (m_it++); }
        inline const_iterator &operator--() { --m_it; return *this; }
        inline const_iterator operator--(int) { return (m_it--); }
        inline const_iterator operator+(int j) const { return (m_it + j); }
        inline const_iterator operator-(int j) const { return operator+(-j); }
        inline const_iterator &operator+=(int j) { return *this = *this + j; }
        inline const_iterator &operator-=(int j) { return *this = *this - j; }

    private:
        typename List::const_iterator m_it;
    };

public:
	OrderedMap()
	{}

	OrderedMap(const OrderedMap &other)
	{
		for (typename List::const_iterator i = other.m_list.begin(), end = other.m_list.end(); i != end; ++i)
			m_map[(*i).first] = m_list.insert(m_list.end(), *i);
	}

	OrderedMap &operator=(const OrderedMap &other)
	{
		m_map.clear();
		m_list.clear();

		for (typename List::const_iterator i = other.m_list.begin(), end = other.m_list.end(); i != end; ++i)
			m_map[(*i).first] = m_list.insert(m_list.end(), *i);

		return *this;
	}

    T &operator[](const Key &key)
    {
    	typename List::iterator &i = m_map[key];

    	if (i == typename List::iterator())
    		return (i = m_list.insert(m_list.end(), Pair(key, T())))->second;
    	else
    		return i->second;
    }
    const T operator[](const Key &key) const { return m_map[key]->second; }

    inline int size() const { return m_list.size(); }
    inline bool isEmpty() const { return m_list.isEmpty(); }

    bool contains(const Key &key) const { return m_map.contains(key); }
    const T value(const Key &key) const { return m_map.value(key)->second; }
    const T value(const Key &key, const T &defaultValue) const { iterator it = m_map.value(key); return m_list.end() == it ? defaultValue : it->second; }

    inline iterator begin() { return m_list.begin(); }
    inline const_iterator begin() const { return m_list.begin(); }
    inline const_iterator constBegin() const { return m_list.constBegin(); }
    inline iterator end() { return m_list.end(); }
    inline const_iterator end() const { return m_list.end(); }
    inline const_iterator constEnd() const { return m_list.constEnd(); }
    iterator erase(iterator it) { m_map.remove((*it).first); return m_list.erase(it); }

    inline void push_back(const Key &key, const T &t) { m_map[key] = m_list.insert(m_list.end(), Pair(key, t)); }
    inline void push_front(const Key &key, const T &t) { m_map[key] = m_list.insert(m_list.begin(), Pair(key, t)); }

    void clear() { m_map.clear(); m_list.clear(); }
    void remove(const Key &key) { m_list.erase(m_map.take(key)); }
    T take(const Key &key) { typename List::iterator it(m_map.take(key)); T res(it->second); m_list.erase(it); return res; }

private:
	Map m_map;
	List m_list;
};

CONTAINERS_NS_END

#endif /* ORDEREDMAP_H_ */
