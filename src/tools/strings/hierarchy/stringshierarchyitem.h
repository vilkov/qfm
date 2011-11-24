#ifndef STRINGSHIERARCHYITEM_H_
#define STRINGSHIERARCHYITEM_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>
#include "stringshierarchy_ns.h"


STRINGS_HIERARCHY_NS_BEGIN

class Item
{
public:
	typedef QMap<QString, Item*>      Container;
	typedef Container::size_type      size_type;
	typedef Container::const_iterator const_iterator;
	enum { InvalidIndex = (size_type)-1 };

public:
	Item(const QString &string, Item *parent = 0) :
		m_parent(parent),
		m_string(string)
	{}

	Item *parent() const { return m_parent; }
	const QString &string() const { return m_string; }

	size_type size() const { return m_items.size(); }
	const_iterator begin() const { return m_items.constBegin(); }
	const_iterator end() const { return m_items.constEnd(); }

private:
	friend class Tree;
	Item *add(const QString &string)
	{
		if (Item *&res = m_items[string])
			return res;
		else
			return (res = new Item(string, this));
	}

private:
	Item *m_parent;
	QString m_string;
	Container m_items;
};

STRINGS_HIERARCHY_NS_END

#endif /* STRINGSHIERARCHYITEM_H_ */
