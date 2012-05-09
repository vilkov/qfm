#ifndef SETTINGSLISTOPTION_H_
#define SETTINGSLISTOPTION_H_

#include <QtCore/QList>
#include "settingsoption.h"


SETTINGS_NS_BEGIN

class ListOption : public Option
{
public:
	typedef QList<Option *>      Container;
	typedef Container::size_type size_type;
	enum { InvalidIndex = (size_type)-1 };

public:
	ListOption(const QString &label, Option *parent = 0) :
		Option(label, parent)
	{}

	virtual Type type() const;

	Option *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }
	size_type indexOf(Option *item) const { return m_items.indexOf(item); }

private:
	Container m_items;
};

SETTINGS_NS_END

#endif /* SETTINGSLISTOPTION_H_ */
