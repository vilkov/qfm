#ifndef IDM_ROOTNODEROOTITEM_H_
#define IDM_ROOTNODEROOTITEM_H_

#include "idm_rootnodeitem.h"


IDM_PLUGIN_NS_BEGIN

class RootNodeRootItem : public RootNodeItem
{
public:
	RootNodeRootItem();

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* RootNodeItem */
	virtual bool isRoot();

private:
	QVariant m_label;
};

IDM_PLUGIN_NS_END

#endif /* IDM_ROOTNODEROOTITEM_H_ */
