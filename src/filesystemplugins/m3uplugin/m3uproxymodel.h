#ifndef M3UPROXYMODEL_H_
#define M3UPROXYMODEL_H_

#include <QtGui/QSortFilterProxyModel>
#include "m3uplugin_ns.h"
#include "items/m3uitem.h"


M3U_PLUGIN_NS_BEGIN

class M3uProxyModel : public QSortFilterProxyModel
{
	Q_DISABLE_COPY(M3uProxyModel)

public:
	M3uProxyModel(QObject *parent = 0);

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

M3U_PLUGIN_NS_END

#endif /* M3UPROXYMODEL_H_ */
