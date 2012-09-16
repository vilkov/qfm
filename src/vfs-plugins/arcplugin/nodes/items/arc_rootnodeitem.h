#ifndef ARC_ROOTNODEITEM_H_
#define ARC_ROOTNODEITEM_H_

#include "arc_nodeitem.h"


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

#endif /* ARC_ROOTNODEITEM_H_ */
