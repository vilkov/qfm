#ifndef IDMROOT_H_
#define IDMROOT_H_

#include "idminfoitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmRoot : public IdmInfoItem
{
public:
	IdmRoot(const Info &info, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;
	virtual bool isRoot() const;

private:
	QString m_label;
};

IDM_PLUGIN_NS_END

#endif /* IDMROOT_H_ */
