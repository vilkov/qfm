#ifndef IDMPLUGIN_H_
#define IDMPLUGIN_H_

#include <QtCore/QString>
#include "actions/idmcreatedbaction.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


IDM_PLUGIN_NS_BEGIN

class Plugin : public IPlugin
{
public:
	Plugin();

	/* ISettings */
	virtual void beginGroup(const QString &name);
	virtual void writeValue(const QString &name, const QVariant &value);
	virtual QVariant readValue(const QString &name, const QVariant &defaultValue = QVariant());
	virtual void endGroup();

	/* IGlobalSettings */
	virtual QString id() const;

	virtual void registered();
	virtual Node *node(const IFileContainer *container, const IFileInfo *file, Node *parent) const;

	static const QString &fileName();

private:
	mutable QString m_error;
	CreateDbAction m_createDbAction;
};

IDM_PLUGIN_NS_END

#endif /* IDMPLUGIN_H_ */
