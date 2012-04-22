#ifndef DEFAULTFOLDERPROXYMODEL_H_
#define DEFAULTFOLDERPROXYMODEL_H_

#include "default_ns.h"
#include "../../filesystem/filesystemproxymodel.h"


DEFAULT_PLUGIN_NS_BEGIN

class FolderProxyModel : public ProxyModel
{
	Q_DISABLE_COPY(FolderProxyModel)

public:
	FolderProxyModel(QObject *parent = 0);

protected:
	virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTFOLDERPROXYMODEL_H_ */
