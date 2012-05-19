#include "filesystemproxymodel.h"


FILE_SYSTEM_NS_BEGIN

ProxyModel::ProxyModel(QObject *parent) :
	QSortFilterProxyModel(parent)
{}

bool ProxyModel::compareByFileNames(const IFileInfo *v1, const IFileInfo *v2)
{
	if (v1->isFile())
		if (v2->isFile())
			return ProxyModel::compareFileNames(v1->fileName(), v2->fileName());
		else
			return false;
	else
		if (v2->isFile())
			return true;
		else
			return ProxyModel::compareFileNames(v1->fileName(), v2->fileName());
}

bool ProxyModel::compareFileNames(const QString &str1, const QString &str2)
{
	return QString::compare(str1, str2, Qt::CaseInsensitive) < 0;
}

FILE_SYSTEM_NS_END
