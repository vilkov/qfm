#ifndef IDMNODEPROXYMODEL_H_
#define IDMNODEPROXYMODEL_H_

#include <QtGui/QSortFilterProxyModel>
#include "../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class IdmNodeProxyModel : public QSortFilterProxyModel
{
	Q_DISABLE_COPY(IdmNodeProxyModel)

public:
	IdmNodeProxyModel(QObject *parent = 0);

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

IDM_PLUGIN_NS_END

#endif /* IDMNODEPROXYMODEL_H_ */
