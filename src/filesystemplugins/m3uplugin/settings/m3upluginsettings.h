#ifndef M3UPLUGINSETTINGS_H_
#define M3UPLUGINSETTINGS_H_

#include <QtCore/QCoreApplication>
#include "../m3uplugin_ns.h"
#include "../../../tools/settings/settingstab.h"


M3U_PLUGIN_NS_BEGIN

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

M3U_PLUGIN_NS_END

#endif /* M3UPLUGINSETTINGS_H_ */
