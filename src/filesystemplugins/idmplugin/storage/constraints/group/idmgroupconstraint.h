#ifndef IDMGROUPCONSTRAINT_H_
#define IDMGROUPCONSTRAINT_H_

#include "../idmbaseconstraint.h"


IDM_PLUGIN_NS_BEGIN

class GroupConstraint : public BaseConstraint
{
public:
	typedef QList<BaseConstraint*> Container;
	typedef Container::size_type   size_type;
	enum { InvalidIndex = (size_type)-1 };
	enum Type
	{
		And,
		Or
	};

public:
	GroupConstraint(Type type);

	/* BaseConstraint */
	virtual bool isGroup() const;
	virtual QString toString() const;

	Type type() const { return m_type; }
	void setType(Type type) { m_type = type; }

	BaseConstraint *at(size_type index) const { return m_items.at(index); }
	size_type size() const { return m_items.size(); }

protected:
	Type m_type;
	Container m_items;
};

IDM_PLUGIN_NS_END

#endif /* IDMGROUPCONSTRAINT_H_ */
