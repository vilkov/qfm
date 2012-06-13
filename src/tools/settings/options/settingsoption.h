#ifndef SETTINGSOPTION_H_
#define SETTINGSOPTION_H_

#include "../settings_ns.h"


SETTINGS_NS_BEGIN

class Option
{
public:
	Option(Option *parent = 0) :
		m_parent(parent)
	{}
	virtual ~Option();

	Option *parent() const { return m_parent; }

    template <typename R> inline
    R *as() { return static_cast<R *>(this); }

    template <typename R> inline
    const R *as() const { return static_cast<const R *>(this); }

private:
	Option *m_parent;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTION_H_ */
