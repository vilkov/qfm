#ifndef ARCPLUGINSETTINGS_H_
#define ARCPLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../arcplugin_ns.h"
#include "../../../tools/settings/settingstab.h"


ARC_PLUGIN_NS_BEGIN

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

ARC_PLUGIN_NS_END

#endif /* ARCPLUGINSETTINGS_H_ */
