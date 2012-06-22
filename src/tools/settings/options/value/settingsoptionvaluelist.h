#ifndef SETTINGSOPTIONVALUELIST_H_
#define SETTINGSOPTIONVALUELIST_H_

#include <QtCore/QList>
#include <QtCore/QString>
#include "../settingsoption.h"


SETTINGS_NS_BEGIN

class OptionValueList : public Option
{
public:
	typedef QList<QString>       Container;
	typedef Container::size_type size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	OptionValueList(const QString &id, Option *parent) :
		Option(parent),
		m_id(id)
	{}

	QString at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(const QString &item) const { return m_items.indexOf(item); }

	void add(const QString &value) { m_items.push_back(value); }
	void remove(size_type index) { m_items.removeAt(index); }
	void clear() { m_items.clear(); }

protected:
    virtual void save(QXmlStreamWriter &stream) const;
    virtual void load(QXmlStreamReader &stream);
    virtual void loadDefault();

protected:
    QString m_id;
	Container m_items;
};

SETTINGS_NS_END

#endif /* SETTINGSOPTIONVALUELIST_H_ */
