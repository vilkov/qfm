#ifndef IDMPROXYMODEL_H_
#define IDMPROXYMODEL_H_

#include <QtGui/QSortFilterProxyModel>
#include "../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class IdmProxyModel : public QSortFilterProxyModel
{
	Q_DISABLE_COPY(IdmProxyModel)

public:
	IdmProxyModel(QObject *parent = 0);

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

IDM_PLUGIN_NS_END

#endif /* IDMPROXYMODEL_H_ */
