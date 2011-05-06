#ifndef FILESYSTEMPROXYMODEL_H_
#define FILESYSTEMPROXYMODEL_H_

#include <QSortFilterProxyModel>
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class ProxyModel : public QSortFilterProxyModel
{
	Q_DISABLE_COPY(ProxyModel)

public:
	ProxyModel(QObject *parent = 0);

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMPROXYMODEL_H_ */
