#ifndef FILESYSTEMFOLDERPROXYMODEL_H_
#define FILESYSTEMFOLDERPROXYMODEL_H_

#include <QSortFilterProxyModel>
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class FolderProxyModel : public QSortFilterProxyModel
{
	Q_DISABLE_COPY(FolderProxyModel)

public:
	FolderProxyModel(QObject *parent = 0);

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMFOLDERPROXYMODEL_H_ */
