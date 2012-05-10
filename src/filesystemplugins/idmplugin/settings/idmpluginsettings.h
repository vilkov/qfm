#ifndef IDMPLUGINSETTINGS_H_
#define IDMPLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../idmplugin_ns.h"
#include "../../../tools/settings/settingstab.h"


IDM_PLUGIN_NS_BEGIN

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

IDM_PLUGIN_NS_END

#endif /* IDMPLUGINSETTINGS_H_ */
