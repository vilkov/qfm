#ifndef SETTINGSVALUELISTOPTION_H_
#define SETTINGSVALUELISTOPTION_H_

#include <QtCore/QList>
#include <QtCore/QVariant>
#include "../settingsoption.h"


SETTINGS_NS_BEGIN

class ValueListOption : public Option
{
public:
	typedef QList<QVariant>      Container;
	typedef Container::size_type size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	ValueListOption(const QString &id, Option *parent = 0) :
		Option(parent),
		m_id(id)
	{}

	QVariant at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(const QVariant &item) const { return m_items.indexOf(item); }

protected:
    virtual void save(QXmlStreamWriter &stream) const;
    virtual void load(QXmlStreamReader &stream);

protected:
    QString m_id;
	Container m_items;
};

SETTINGS_NS_END

#endif /* SETTINGSVALUELISTOPTION_H_ */
