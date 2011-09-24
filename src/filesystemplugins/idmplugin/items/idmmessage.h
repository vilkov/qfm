#ifndef IDMMESSAGE_H_
#define IDMMESSAGE_H_

#include <QtCore/QString>
#include "idmbaseitem.h"


IDM_PLUGIN_NS_BEGIN

class IdmMessage : public IdmBaseItem
{
public:
	IdmMessage(const QString &message, IdmItem *parent = 0);

	/* IdmItem */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* IBaseItem */
	virtual bool isRoot() const;
	virtual bool isList() const;
	virtual bool isMenuItem() const;
	virtual bool isValueItem() const;
	virtual bool isEntityItem() const;

private:
	QVariant m_message;
};

IDM_PLUGIN_NS_END

#endif /* IDMMESSAGE_H_ */
