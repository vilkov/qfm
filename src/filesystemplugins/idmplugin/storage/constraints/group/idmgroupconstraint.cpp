#include "idmgroupconstraint.h"


IDM_PLUGIN_NS_BEGIN

GroupConstraint::GroupConstraint(Type type, BaseConstraint *parent) :
	BaseConstraint(parent),
	m_type(type)
{}

GroupConstraint::~GroupConstraint()
{
	qDeleteAll(m_items);
}

bool GroupConstraint::isGroup() const
{
	return true;
}

QString GroupConstraint::toString() const
{
	return QString();
}

QString GroupConstraint::typeToString(Type type)
{
	switch (type)
	{
		case GroupConstraint::And:
			return tr("AND");

		case GroupConstraint::Or:
			return tr("OR");
	}
}

void GroupConstraint::add(BaseConstraint *constraint)
{
	m_items.push_back(constraint);
}

void GroupConstraint::insert(size_type index, BaseConstraint *constraint)
{
	m_items.insert(index, constraint);
}

void GroupConstraint::swap(size_type index1, size_type index2)
{
	m_items.swap(index1, index2);
}

void GroupConstraint::remove(size_type index)
{
	delete m_items.takeAt(index);
}

IDM_PLUGIN_NS_END
