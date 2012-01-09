#ifndef ARCNODEROOTITEM_H_
#define ARCNODEROOTITEM_H_

#include "arcnodeitem.h"


ARC_PLUGIN_NS_BEGIN

class ArcNodeRootItem : public ArcNodeItem
{
public:
	ArcNodeRootItem(Base *parent = 0);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* IArcNodeItem */
	virtual bool isRoot();
	virtual bool isDir();

private:
	QVariant m_label;
};

ARC_PLUGIN_NS_END

#endif /* ARCNODEROOTITEM_H_ */
