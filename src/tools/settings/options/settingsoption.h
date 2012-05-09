#ifndef SETTINGSOPTION_H_
#define SETTINGSOPTION_H_

#include <QtCore/QString>
#include <QtCore/QVariant>
#include "../settings_ns.h"


SETTINGS_NS_BEGIN

class Option
{
public:
	enum Type
	{
		Value,
		List,
		Group,
		Page
	};

public:
	Option(const QString &label, Option *parent = 0) :
		m_label(label),
		m_parent(parent)
	{}
	virtual ~Option();

	virtual Type type() const = 0;

	virtual bool isEnabled() const = 0;
	virtual void setEnabled(bool value) = 0;

	Option *parent() const { return m_parent; }
	const QString &label() const { return m_label; }

    template <typename R> inline
    R *as() { return static_cast<R *>(this); }

    template <typename R> inline
    const R *as() const { return static_cast<const R *>(this); }

private:
	QString m_label;
	Option *m_parent;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTION_H_ */
