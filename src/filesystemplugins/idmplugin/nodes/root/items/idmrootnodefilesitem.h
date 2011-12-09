#ifndef IDMROOTNODEFILESITEM_H_
#define IDMROOTNODEFILESITEM_H_

#include <QtCore/QCoreApplication>
#include "idmrootnodeitem.h"
#include "../../../../../filesystem/filesystemnode.h"


IDM_PLUGIN_NS_BEGIN

class RootNodeFilesItem : public RootNodeItem
{
	Q_DECLARE_TR_FUNCTIONS(RootNodeFilesItem)

public:
	RootNodeFilesItem();

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* IRootNodeItem */
	virtual bool isRoot();
	virtual bool isFiles();
	virtual bool isEntity();
	virtual bool isProperty();

	Node *node() const { return m_node; }
	void setNode(Node *node) { m_node = node; }

private:
	QVariant m_label;
	Node *m_node;
};

IDM_PLUGIN_NS_END

#endif /* IDMROOTNODEFILESITEM_H_ */
