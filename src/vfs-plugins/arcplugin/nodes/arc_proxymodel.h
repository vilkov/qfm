#ifndef ARC_PROXYMODEL_H_
#define ARC_PROXYMODEL_H_

#include <QtGui/QSortFilterProxyModel>
#include "../arc_ns.h"


ARC_PLUGIN_NS_BEGIN

class ProxyModel : public QSortFilterProxyModel
{
	Q_DISABLE_COPY(ProxyModel)

public:
	ProxyModel(QObject *parent = 0);

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

ARC_PLUGIN_NS_END

#endif /* ARC_PROXYMODEL_H_ */
