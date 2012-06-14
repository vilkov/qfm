#ifndef SETTINGSOPTION_H_
#define SETTINGSOPTION_H_

#include <QtCore/QString>
#include <QtXml/QXmlStreamWriter>
#include <QtXml/QXmlStreamReader>
#include "../settings_ns.h"


SETTINGS_NS_BEGIN

class Option
{
public:
	Option(const QString &id, Option *parent = 0) :
		m_parent(parent),
		m_id(id)
	{}
	virtual ~Option();

	Option *parent() const { return m_parent; }
	const QString &id() const { return m_id; }

    template <typename R> inline
    R *as() { return static_cast<R *>(this); }

    template <typename R> inline
    const R *as() const { return static_cast<const R *>(this); }

protected:
    friend class Scope;
    virtual void save(QXmlStreamWriter &stream) const = 0;
    virtual void load(QXmlStreamReader &stream) = 0;

private:
	Option *m_parent;
	QString m_id;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTION_H_ */
