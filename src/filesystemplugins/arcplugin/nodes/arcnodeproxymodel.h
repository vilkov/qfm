#ifndef ARCNODEPROXYMODEL_H_
#define ARCNODEPROXYMODEL_H_

#include <QtGui/QSortFilterProxyModel>
#include "../arcplugin_ns.h"


ARC_PLUGIN_NS_BEGIN

class ArcNodeProxyModel : public QSortFilterProxyModel
{
	Q_DISABLE_COPY(ArcNodeProxyModel)

public:
	ArcNodeProxyModel(QObject *parent = 0);

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

ARC_PLUGIN_NS_END

#endif /* ARCNODEPROXYMODEL_H_ */
