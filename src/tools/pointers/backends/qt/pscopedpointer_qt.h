#ifndef PSCOPEDPOINTER_QT_H_
#define PSCOPEDPOINTER_QT_H_

#include <QtCore/QScopedPointer>


template <typename T>
class PScopedPointer
{
public:
    explicit PScopedPointer(T *p = 0) :
    	m_data(p)
    {}

    T &operator*() const { return *m_data; }
    T *operator->() const { return m_data.operator->(); }
    T *data() const { return m_data.get(); }

    void reset(T *p = 0) { m_data.reset(p); }
    void swap(PScopedPointer &other) { m_data.swap(other.m_data); }
    T *take() { return m_data.take(); }

private:
    PScopedPointer(PScopedPointer const &);
    PScopedPointer &operator=(PScopedPointer const &);

    void operator==(PScopedPointer const &) const;
    void operator!=(PScopedPointer const &) const;

private:
    QScopedPointer<T> m_data;
};

#endif /* PSCOPEDPOINTER_QT_H_ */
