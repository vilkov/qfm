#ifndef DEFAULTPROXYMODEL_H_
#define DEFAULTPROXYMODEL_H_

#include <vfs/vfs_proxymodel.h>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class ProxyModel : public ::VFS::ProxyModel
{
	Q_DISABLE_COPY(ProxyModel)

public:
	ProxyModel(QObject *parent = 0);

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTPROXYMODEL_H_ */
