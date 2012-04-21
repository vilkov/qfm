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
    operator bool() const { return !m_data.isNull(); }

    class_type &operator=(T *p) { reset(p); return *this; }

    bool reset(T *p = 0) { m_data.reset(p); return !m_data.isNull(); }
    void swap(class_type &other) { m_data.swap(other.m_data); }
    T *take() { return m_data.take(); }

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
