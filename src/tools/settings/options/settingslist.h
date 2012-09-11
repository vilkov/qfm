#ifndef SETTINGSLIST_H_
#define SETTINGSLIST_H_

#include "settingsoption.h"
#include "../../containers/orderedmap.h"


SETTINGS_NS_BEGIN

class List : public Option
{
public:
	typedef Containers::OrderedMap<QStringRef, Option *> Container;
	typedef Container::const_iterator                    const_iterator;

public:
	List(const Id &id, Option *parent = 0) :
		Option(id, parent)
	{}

	bool isEmpty() const { return m_items.isEmpty(); }

	Option *find(const Id &id) const { return m_items.value(&id); }
	const_iterator begin() const { return m_items.constBegin(); }
	const_iterator end() const { return m_items.constEnd(); }

protected:
	Container m_items;
};

SETTINGS_NS_END

#endif /* SETTINGSLIST_H_ */
