#ifndef M3UPROXYMODEL_H_
#define M3UPROXYMODEL_H_

#include <QSortFilterProxyModel>


class M3uProxyModel : public QSortFilterProxyModel
{
	Q_DISABLE_COPY(M3uProxyModel)

public:
	M3uProxyModel(QObject *parent = 0);

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

#endif /* M3UPROXYMODEL_H_ */
