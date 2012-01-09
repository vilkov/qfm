#ifndef TREEMODELITEM_H_
#define TREEMODELITEM_H_

#include <QtCore/QVariant>
#include "../treemodel_ns.h"


MODELS_TREE_NS_BEGIN

class Item
{
public:
	typedef Item Base;

public:
	Item(Base *parent);
	virtual ~Item();

	Base *parent() const { return m_parent; }

	virtual bool isList() const;
	virtual QVariant data(qint32 column, qint32 role) const = 0;

private:
	Base *m_parent;
};

MODELS_TREE_NS_END

#endif /* TREEMODELITEM_H_ */
