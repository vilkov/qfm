#ifndef IDMPLUGINSETTINGS_H_
#define IDMPLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../idmplugin_ns.h"


IDM_PLUGIN_NS_BEGIN

class Settings
{
	Q_DECLARE_TR_FUNCTIONS(Settings)

public:
	Settings();

	const QByteArray &id() const;
	const QString &name() const;
	QWidget *centralWidget() const;

private:
	QByteArray m_id;
	QString m_name;
};

IDM_PLUGIN_NS_END

#endif /* IDMPLUGINSETTINGS_H_ */
