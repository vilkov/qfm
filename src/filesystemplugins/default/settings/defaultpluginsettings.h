#ifndef DEFAULTPLUGINSETTINGS_H_
#define DEFAULTPLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../default_ns.h"
#include "../../../tools/settings/settingstab.h"


DEFAULT_PLUGIN_NS_BEGIN

class Settings : public ::Tools::Settings::Tab
{
	Q_DECLARE_TR_FUNCTIONS(Settings)

public:
	Settings();

	virtual const QByteArray &id() const;
	virtual const QString &name() const;
	virtual QWidget *centralWidget() const;

private:
	QByteArray m_id;
	QString m_name;
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTPLUGINSETTINGS_H_ */
