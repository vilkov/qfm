#ifndef M3UPLUGIN_H_
#define M3UPLUGIN_H_

#include "m3uplugin_ns.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


M3U_PLUGIN_NS_BEGIN

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

private:
	QString m_identity;
};

M3U_PLUGIN_NS_END

#endif /* M3UPLUGIN_H_ */
