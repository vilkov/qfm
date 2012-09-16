#ifndef IDM_VALUELISTPROXYMODEL_H_
#define IDM_VALUELISTPROXYMODEL_H_

#include <QtGui/QSortFilterProxyModel>
#include "../../../idm_ns.h"


IDM_PLUGIN_NS_BEGIN

class ValueListProxyModel : public QSortFilterProxyModel
{
	Q_DISABLE_COPY(ValueListProxyModel)

public:
	ValueListProxyModel(QObject *parent = 0);

	void setFilter(const QString &filter);

protected:
	virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
	QString m_filter;
};

IDM_PLUGIN_NS_END

#endif /* IDM_VALUELISTPROXYMODEL_H_ */
