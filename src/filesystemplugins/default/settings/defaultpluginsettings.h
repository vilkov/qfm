#ifndef DEFAULTPLUGINSETTINGS_H_
#define DEFAULTPLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

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

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTPLUGINSETTINGS_H_ */
