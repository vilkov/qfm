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
#ifndef MEMORY_HOLDER_H_
#define MEMORY_HOLDER_H_

#include <QtCore/QExplicitlySharedDataPointer>
#include "memory_ns.h"


MEMORY_NS_BEGIN

template <typename T>
class Holder : public QExplicitlySharedDataPointer<T>
{
public:
    inline Holder() :
        QExplicitlySharedDataPointer<T>()
    {}

    inline explicit Holder(T *data) :
        QExplicitlySharedDataPointer<T>(data)
    {}

    inline bool operator<(const T *other) const { return QExplicitlySharedDataPointer<T>::data() < other; }
    inline bool operator<(const Holder<T> &other) const { return QExplicitlySharedDataPointer<T>::data() < other.data(); }

    inline bool operator==(const T *other) const { return QExplicitlySharedDataPointer<T>::data() == other; }
    inline bool operator==(const Holder<T> &other) const { return QExplicitlySharedDataPointer<T>::data() == other.data(); }

    inline Holder<T> &operator=(T *o) { QExplicitlySharedDataPointer<T>::operator=(o); return *this; }
    inline Holder<T> &operator=(const Holder<T> &o) { QExplicitlySharedDataPointer<T>::operator=(o); return *this; }

    template <typename R> inline
    R *as() const { return static_cast<R *>(QExplicitlySharedDataPointer<T>::data()); }

    template <typename R> inline
    const R *as_const() const { return static_cast<const R *>(QExplicitlySharedDataPointer<T>::data()); }

    template <typename Container, typename V>
    inline static typename Container::size_type indexOf(const Container &container, const V &val, const typename Container::size_type defVal = -1)
    {
        typename Container::size_type q = 0;
        typename Container::size_type res = defVal;

        for (typename Container::const_iterator i = container.begin(), end = container.end(); i != end; ++i, ++q)
            if ((*i) == val)
            {
                res = q;
                break;
            }

        return res;
    }
};

MEMORY_NS_END

#endif /* MEMORY_HOLDER_H_ */
