#ifndef TREEITEM_H_
#define TREEITEM_H_

#include <QtCore/QVariant>
#include "../../models_ns.h"


MODELS_NS_BEGIN

class TreeItem
{
public:
	TreeItem(TreeItem *parent);
	virtual ~TreeItem();

	TreeItem *parent() const { return m_parent; }

	virtual bool isList() const = 0;
	virtual QVariant data(qint32 column, qint32 role) const = 0;

private:
	TreeItem *m_parent;
};

MODELS_NS_END

#endif /* TREEITEM_H_ */
