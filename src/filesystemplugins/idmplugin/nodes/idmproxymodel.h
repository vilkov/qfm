#ifndef IDMPROXYMODEL_H_
#define IDMPROXYMODEL_H_

#include <QtGui/QSortFilterProxyModel>
#include "../../../filesystem/filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class IdmProxyModel : public QSortFilterProxyModel
{
	Q_DISABLE_COPY(IdmProxyModel)

public:
	IdmProxyModel(QObject *parent = 0);

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

FILE_SYSTEM_NS_END

#endif /* IDMPROXYMODEL_H_ */
