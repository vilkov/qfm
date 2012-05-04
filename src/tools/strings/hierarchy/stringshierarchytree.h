#ifndef STRINGSHIERARCHYTREE_H_
#define STRINGSHIERARCHYTREE_H_

#include "stringshierarchyitem.h"


STRINGS_HIERARCHY_NS_BEGIN

class Tree : public Item
{
public:
	Tree(QChar delimeter) :
		Item(QString(), 0),
		m_delimeter(delimeter)
	{}

	QChar delimeter() const { return m_delimeter; }

	void add(const QString &string)
	{
		int index1 = 0;
		Item *parent = this;
		QString last;

		for (int index2 = string.indexOf(m_delimeter); index2 != -1; index2 = string.indexOf(m_delimeter, index1 = (++index2)))
			parent = parent->add(string.mid(index1, index2 - index1));

		if (!(last = string.mid(index1, string.size() - index1)).isEmpty())
			parent->add(last);
	}

	QString make(Item *item) const
	{
		QString res = item->string();

		for (item = item->parent(); item; item = item->parent())
			res.prepend(m_delimeter).prepend(item->string());

		return res.remove(0, 1);
	}

private:
	QChar m_delimeter;
};

STRINGS_HIERARCHY_NS_END

#endif /* STRINGSHIERARCHYTREE_H_ */
