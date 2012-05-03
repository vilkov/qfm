#ifndef DEFAULTPLUGIN_H_
#define DEFAULTPLUGIN_H_

#include <QtCore/QSettings>
#include "default_ns.h"
#include "../../filesystem/interfaces/filesystemiplugin.h"


DEFAULT_PLUGIN_NS_BEGIN

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
	QSettings m_settings;
	mutable QString m_error;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTPLUGIN_H_ */
