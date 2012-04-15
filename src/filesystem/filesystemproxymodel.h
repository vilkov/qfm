#ifndef FILESYSTEMPROXYMODEL_H_
#define FILESYSTEMPROXYMODEL_H_

#include <QtGui/QSortFilterProxyModel>
#include "filesystem_ns.h"
#include "interfaces/filesystemifileinfo.h"


FILE_SYSTEM_NS_BEGIN

class ProxyModel : public QSortFilterProxyModel
{
	Q_DISABLE_COPY(ProxyModel)

public:
	ProxyModel(QObject *parent = 0);

	static bool compareByFileNames(const IFileInfo *v1, const IFileInfo *v2);
	static bool compareFileNames(const QString &str1, const QString &str2);
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMPROXYMODEL_H_ */
