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
#ifndef PSCOPEDPOINTER_BOOST_H_
#define PSCOPEDPOINTER_BOOST_H_

#include <boost/scoped_ptr.hpp>


template <typename T>
class PScopedPointer
{
public:
    PScopedPointer(T *p = 0) :
    	m_data(p)
    {}

    T &operator*() const { return m_data.operator*(); }
    T *operator->() const { return m_data.operator->(); }
    T *data() const { return m_data.get(); }
    operator bool() const { return !m_data.isNull(); }

    class_type &operator=(T *p) { reset(p); return *this; }

    void reset(T *p = 0) { m_data.reset(p); }
    void swap(PScopedPointer &other) { m_data.swap(other.m_data); }
    T *take() { T *res = m_data.get(); m_data.reset(); return res; }

private:
    PScopedPointer(PScopedPointer const &);
    PScopedPointer &operator=(PScopedPointer const &);

    void operator==(PScopedPointer const &) const;
    void operator!=(PScopedPointer const &) const;

private:
	boost::scoped_ptr<T> m_data;
};

#endif /* PSCOPEDPOINTER_BOOST_H_ */
