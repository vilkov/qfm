#ifndef ARCROOTNODEITEM_H_
#define ARCROOTNODEITEM_H_

#include "arcnodeitem.h"


ARC_PLUGIN_NS_BEGIN

class RootNodeItem : public NodeItem
{
public:
	RootNodeItem(Base *parent = 0);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* ArcNodeItem */
	virtual bool isRoot() const;

private:
	QVariant m_label;
};

ARC_PLUGIN_NS_END

#endif /* ARCROOTNODEITEM_H_ */
