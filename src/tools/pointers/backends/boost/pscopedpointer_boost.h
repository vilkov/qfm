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
