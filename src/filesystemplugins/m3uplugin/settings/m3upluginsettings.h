#ifndef M3UPLUGINSETTINGS_H_
#define M3UPLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../m3uplugin_ns.h"


M3U_PLUGIN_NS_BEGIN

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

M3U_PLUGIN_NS_END

#endif /* M3UPLUGINSETTINGS_H_ */
