#ifndef FILESYSTEMPROXYMODEL_H_
#define FILESYSTEMPROXYMODEL_H_

#include <QSortFilterProxyModel>


class FileSystemProxyModel : public QSortFilterProxyModel
{
	Q_OBJECT

public:
	FileSystemProxyModel(QObject *parent = 0);

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

#endif /* FILESYSTEMPROXYMODEL_H_ */
