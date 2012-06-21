#ifndef ARCPLUGINSETTINGS_H_
#define ARCPLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../arcplugin_ns.h"


ARC_PLUGIN_NS_BEGIN

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

ARC_PLUGIN_NS_END

#endif /* ARCPLUGINSETTINGS_H_ */
