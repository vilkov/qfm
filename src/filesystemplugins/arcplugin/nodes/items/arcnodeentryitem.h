#ifndef ARCNODEENTRYITEM_H_
#define ARCNODEENTRYITEM_H_

#include "arcnodeitem.h"


ARC_PLUGIN_NS_BEGIN

class ArcNodeEntryItem : public ArcNodeItem
{
public:
	ArcNodeEntryItem(const QString &fileName, Base *parent = 0);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* IArcNodeItem */
	virtual bool isRoot();
	virtual bool isDir();

	const QString &fileName() const { return m_fileName; }

private:
	QString m_fileName;
};

ARC_PLUGIN_NS_END

#endif /* ARCNODEENTRYITEM_H_ */
