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
#ifndef PSCOPEDPOINTER_QT_H_
#define PSCOPEDPOINTER_QT_H_

#include <QtCore/QScopedPointer>


template <typename T>
class PScopedPointer
{
public:
	typedef PScopedPointer<T> class_type;

public:
    explicit PScopedPointer(T *p = 0) :
    	m_data(p)
    {}

    T &operator*() const { return *m_data; }
    T *operator->() const { return m_data.operator->(); }
    T *data() const { return m_data.data(); }
    const T *const_data() const { return m_data.data(); }
    operator bool() const { return !m_data.isNull(); }
    operator const T *() const { return m_data.data(); }

    class_type &operator=(T *p) { reset(p); return *this; }

    bool reset(T *p = 0) { m_data.reset(p); return !m_data.isNull(); }
    void reset(class_type &other) { class_type tmp; tmp.swap(*this); swap(other); }
    void swap(class_type &other) { m_data.swap(other.m_data); }
    T *take() { return m_data.take(); }
    void release() { m_data.take(); }

    template <typename R> inline
    const R *as() const { return static_cast<const R *>(m_data.data()); }

    template <typename R> inline
    R *as() { return static_cast<R *>(m_data.data()); }

private:
    PScopedPointer(const class_type &);
    class_type &operator=(const class_type &);

    void operator==(const class_type &) const;
    void operator!=(const class_type &) const;

private:
    QScopedPointer<T> m_data;
};


template <typename T>
class PScopedPointerArray
{
public:
	typedef PScopedPointerArray<T> class_type;

public:
    explicit PScopedPointerArray(T *p = 0) :
    	m_data(p)
    {}

    T &operator*() const { return *m_data; }
    T *operator->() const { return m_data.operator->(); }
    T *data() const { return m_data.data(); }
    operator bool() const { return !m_data.isNull(); }

    class_type &operator=(T *p) { reset(p); return *this; }

    bool reset(T *p = 0) { m_data.reset(p);  return !m_data.isNull(); }
    void swap(class_type &other) { m_data.swap(other.m_data); }
    T *take() { return m_data.take(); }

private:
    PScopedPointerArray(const class_type &);
    class_type &operator=(const class_type &);

    void operator==(const class_type &) const;
    void operator!=(const class_type &) const;

private:
    QScopedPointer<T, QScopedPointerArrayDeleter<T> > m_data;
};

#endif /* PSCOPEDPOINTER_QT_H_ */
