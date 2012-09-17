#include "vfs_proxymodel.h"
#include "../../tools/strings/strnatcmp.h"


VFS_NS_BEGIN

ProxyModel::ProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool ProxyModel::compareByFileNames(const IFileInfo *v1, const IFileInfo *v2)
{
	if (v1->isFile())
		if (v2->isFile())
			return compareFileNames(v1->fileName(), v2->fileName());
		else
			return false;
	else
		if (v2->isFile())
			return true;
		else
			return compareFileNames(v1->fileName(), v2->fileName());
}

bool ProxyModel::compareFileNames(const QString &str1, const QString &str2)
{
	return strnatcasecmp(str1.data(), str2.data()) < 0;
}

VFS_NS_END
