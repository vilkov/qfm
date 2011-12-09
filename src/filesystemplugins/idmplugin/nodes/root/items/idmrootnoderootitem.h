#ifndef IDMROOTNODEROOTITEM_H_
#define IDMROOTNODEROOTITEM_H_

#include "idmrootnodeitem.h"


IDM_PLUGIN_NS_BEGIN

class RootNodeRootItem : public RootNodeItem
{
public:
	RootNodeRootItem();

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* IRootNodeItem */
	virtual bool isRoot();
	virtual bool isFiles();
	virtual bool isEntity();
	virtual bool isProperty();

private:
	QVariant m_label;
};

IDM_PLUGIN_NS_END

#endif /* IDMROOTNODEROOTITEM_H_ */
