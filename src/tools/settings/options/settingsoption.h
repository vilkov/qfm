#ifndef SETTINGSOPTION_H_
#define SETTINGSOPTION_H_

#include <QtXml/QXmlStreamWriter>
#include <QtXml/QXmlStreamReader>
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

protected:
    friend class Scope;
    friend class Container;
    virtual void save(QXmlStreamWriter &stream) const = 0;
    virtual void load(QXmlStreamReader &stream) = 0;

private:
	Option *m_parent;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTION_H_ */
