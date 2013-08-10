/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2013 Dmitriy Vilkov, <dav.daemon@gmail.com>
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
#ifndef ORDEREDMAP_H_
#define ORDEREDMAP_H_

#include <QtCore/QMap>
#include <QtCore/QPair>
#include <QtCore/QLinkedList>
#include "containers_ns.h"
#include "../platform/platform.h"


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
        friend class OrderedMap<Key, T>;
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
        inline const T &value() const { return m_it->second; }
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
        friend class OrderedMap<Key, T>;
        typename List::const_iterator m_it;
    };

public:
    inline OrderedMap()
    {}

#if PLATFORM_COMPILER_SUPPORTS(MOVE_SEMANTIC)
    inline OrderedMap(OrderedMap &&other) :
        m_map(std::move(other.m_map)),
        m_list(std::move(other.m_list))
    {}
#endif

    inline OrderedMap(const OrderedMap &other)
    {
        for (typename List::const_iterator i = other.m_list.begin(), end = other.m_list.end(); i != end; ++i)
            m_map[(*i).first] = m_list.insert(m_list.end(), *i);
    }

    inline bool operator==(const OrderedMap &other) const
    {
        if (size() != other.size())
            return false;
        else
            for (typename List::const_iterator other_i = other.m_list.begin(), this_i = m_list.begin(), end = m_list.end(); this_i != end; ++this_i, ++other_i)
                if ((*this_i) != (*other_i))
                    return false;

        return true;
    }

    inline OrderedMap &operator=(const OrderedMap &other)
    {
        m_map.clear();
        m_list.clear();

        for (typename List::const_iterator i = other.m_list.begin(), end = other.m_list.end(); i != end; ++i)
            m_map[(*i).first] = m_list.insert(m_list.end(), *i);

        return *this;
    }

    inline T &operator[](const Key &key)
    {
        typename List::iterator &i = m_map[key];

        if (i == typename List::iterator())
            return (i = m_list.insert(m_list.end(), Pair(key, T())))->second;
        else
            return i->second;
    }
    inline const T operator[](const Key &key) const
    {
        const typename List::iterator i(m_map[key]);
        return i == typename List::iterator() ? T() : i->second;
    }

    inline int size() const { return m_map.size(); }
    inline bool isEmpty() const { return m_list.isEmpty(); }
    inline bool contains(const Key &key) const { return m_map.contains(key); }

    inline const T value(const Key &key) const
    {
        const typename List::iterator i(m_map.value(key));
        return i == typename List::iterator() ? T() : i->second;
    }
    inline const T value(const Key &key, const T &defaultValue) const
    {
        const typename List::iterator i(m_map.value(key));
        return i == typename List::iterator() ? defaultValue : i->second;
    }

    inline iterator begin() { return m_list.begin(); }
    inline const_iterator begin() const { return m_list.begin(); }
    inline const_iterator constBegin() const { return m_list.constBegin(); }
    inline iterator end() { return m_list.end(); }
    inline const_iterator end() const { return m_list.end(); }
    inline const_iterator constEnd() const { return m_list.constEnd(); }
    inline iterator erase(iterator it) { m_map.erase(m_map.find(it.key())); return m_list.erase(it.m_it); }

    inline void push_back(const Key &key, const T &t)
    {
        m_map[key] = m_list.insert(m_list.end(), Pair(key, t));
    }
    inline void push_front(const Key &key, const T &t)
    {
        m_map[key] = m_list.insert(m_list.begin(), Pair(key, t));
    }

#if PLATFORM_COMPILER_SUPPORTS(MOVE_SEMANTIC)
    inline void push_back(const Key &key, T &&t)
    {
        m_map[key] = m_list.insert(m_list.end(), Pair(key, std::move(t)));
    }
    inline void push_front(const Key &key, T &&t)
    {
        m_map[key] = m_list.insert(m_list.begin(), Pair(key, std::move(t)));
    }
#endif

    inline void clear()
    {
        m_map.clear();
        m_list.clear();
    }
    inline void remove(const Key &key)
    {
        typename List::iterator i(m_map.take(key));

        if (i != typename List::iterator())
            m_list.erase(i);
    }
    inline T take(const Key &key)
    {
        typename List::iterator i(m_map.take(key));

        if (i != typename List::iterator())
        {
            T res(i->second);
            m_list.erase(i);
            return res;
        }

        return T();
    }

private:
    Map m_map;
    List m_list;
};

CONTAINERS_NS_END

#endif /* ORDEREDMAP_H_ */
