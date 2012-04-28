#ifndef DEFAULTNODEROOTITEM_H_
#define DEFAULTNODEROOTITEM_H_

#include "defaultnodeitem.h"


DEFAULT_PLUGIN_NS_BEGIN

class DefaultNodeRootItem : public DefaultNodeItem
{
public:
	DefaultNodeRootItem(Base *parent = 0);

	/* ::Tools::Models::Tree::Item */
	virtual QVariant data(qint32 column, qint32 role) const;

	virtual bool isRootItem() const;

	const QVariant &label() const { return m_label; }

private:
	QVariant m_label;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTNODEROOTITEM_H_ */
