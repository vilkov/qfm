#ifndef SETTINGSLIST_H_
#define SETTINGSLIST_H_

#include <QtCore/QList>
#include "settingsoption.h"


SETTINGS_NS_BEGIN

class List : public Option
{
public:
	typedef QList<Option *>      Container;
	typedef Container::size_type size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	List(Option *parent = 0) :
		Option(parent)
	{}

	bool isEmpty() const { return m_items.isEmpty(); }

	Option *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(Option *item) const { return m_items.indexOf(item); }

protected:
	Container m_items;
};

SETTINGS_NS_END

#endif /* SETTINGSLIST_H_ */
