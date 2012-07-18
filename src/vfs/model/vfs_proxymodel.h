#ifndef VFS_PROXYMODEL_H_
#define VFS_PROXYMODEL_H_

#include <QtGui/QSortFilterProxyModel>
#include "../interfaces/vfs_ifileinfo.h"


VFS_NS_BEGIN

class ProxyModel : public QSortFilterProxyModel
{
	Q_DISABLE_COPY(ProxyModel)

public:
	ProxyModel(QObject *parent = 0);

	static bool compareByFileNames(const IFileInfo *v1, const IFileInfo *v2);
	static bool compareFileNames(const QString &str1, const QString &str2);
};

VFS_NS_END

#endif /* VFS_PROXYMODEL_H_ */
