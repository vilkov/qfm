#include "applicationsettings.h"
#include "configoptions/emailconfigoption.h"
#include "configoptions/colorconfigoption.h"
#include "configoptions/checkconfigoption.h"
#include "configoptions/passwordconfigoption.h"
#include "../mainwindow/foldersview/foldersview.h"
#include "../tools/settings/configoptions/configoptionvalue.h"
#include "../tools/settings/configoptions/configoptiongroup.h"
#include "../tools/settings/constraints/configoptionconstraintrange.h"
#include "../tools/settings/constraints/configoptionconstrainturl.h"
#include "../tools/settings/constraints/configoptionconstraintlessthan.h"
#include "../tools/settings/constraints/configoptionconstraintempty.h"
#include "../tools/settings/constraints/configoptionconstraintemail.h"
#include "../application.h"
#include <QtCore/QDir>
#include <QtGui/QMessageBox>
#include <QtGui/QColorDialog>
#include <QtGui/QDesktopServices>

#define FROM_RAW_DATA(a) QByteArray::fromRawData(a, qstrlen(a))


ApplicationSettings::ApplicationSettings(QObject *parent) :
	QObject(parent), Settings(tr("Settings"), QDir(storageLocation()).absoluteFilePath(QString::fromLatin1("settings.ini")), createSettins())
{}

QVariant ApplicationSettings::readValue(const qint32 &settingId)
{
	QMutexLocker locker(&m_mutex);
	return Settings::readValue(settingId);
}

void ApplicationSettings::saveState(QWidget *widget)
{
	beginGroup(widget->objectName());
	setValue(QString::fromLatin1("Geometry"), widget->saveGeometry());
	endGroup();
}

void ApplicationSettings::saveState(QWidget *widget, QAbstractItemView *view)
{
	QString geometry = QString::fromLatin1("Geometry");

	beginGroup(widget->objectName());
	setValue(geometry, widget->saveGeometry());
	setValue(geometry.append(QChar('.')).append(view->objectName()), widget->saveGeometry());
	endGroup();
}

void ApplicationSettings::saveState(QWidget *widget, QSplitter *splitter, QAbstractItemView *view)
{
	QString geometry = QString::fromLatin1("Geometry");

	beginGroup(widget->objectName());
	setValue(geometry, widget->saveGeometry());
	setValue(QString::fromLatin1("Splitter"), splitter->saveState());
	setValue(geometry.append(QChar('.')).append(view->objectName()), widget->saveGeometry());
	endGroup();
}

void ApplicationSettings::loadState(QWidget *widget)
{
	beginGroup(widget->objectName());
	widget->restoreGeometry(value(QString::fromLatin1("Geometry")).toByteArray());
	endGroup();
}

void ApplicationSettings::loadState(QWidget *widget, QAbstractItemView *view)
{
	QString geometry = QString::fromLatin1("Geometry");

	beginGroup(widget->objectName());
	widget->restoreGeometry(value(geometry).toByteArray());
	view->restoreGeometry(value(geometry.append(QChar('.')).append(view->objectName())).toByteArray());
	endGroup();
}

void ApplicationSettings::loadState(QWidget *widget, QSplitter *splitter, QAbstractItemView *view)
{
	QString geometry = QString::fromLatin1("Geometry");

	beginGroup(widget->objectName());
	widget->restoreGeometry(value(geometry).toByteArray());
	splitter->restoreState(value(QString::fromLatin1("Splitter")).toByteArray());
	view->restoreGeometry(value(geometry.append(QChar('.')).append(view->objectName())).toByteArray());
	endGroup();
}

QString ApplicationSettings::storageLocation()
{
	return Settings::storageLocation(QString::fromLatin1(".qfm"));
}

ApplicationSettings::Argument ApplicationSettings::createSettins()
{
	SettingsList settings;
	ConstraintsList constraints;
	ConfigOptionValue *setting1;
	ConfigOptionValue *setting2;
	ConfigOptionGroup *group;
	ConfigOptionGroup *subGroup;
	ConfigOptionValue *networkTimeout;
	ConfigOptionValue *webServerDataRequestTimeout;
	ConfigOptionValue *databaseReconnectTimeout;

	/* Common */
	settings.push_back(group = new ConfigOptionGroup(tr("Common")));

	setting1 = group->add<QLineEdit>(StorageLocation, tr("Storage location"), QDir(QDesktopServices::storageLocation(QDesktopServices::DataLocation)).absolutePath());
	constraints.push_back(new ConfigOptionConstraintEmpty(setting1));

//	setting1 = group->add<QSpinBox>(NumberOfThreads, tr("Number of threads"), 10);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 1, 20));
//
//	setting1 = group->add<QSpinBox>(TasksPerThread, tr("Tasks per thread"), 20);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 10, 100));
//
//	setting1 = group->add<QSpinBox>(KeepCacheRecordsInMemory, tr("Keep cache records in memory"), 500);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 100, 1000));
//
//	setting1 = group->add<QSpinBox>(IdentityCacheRecordsLifeTime, tr("Identity cache records lifetime (sec)"), 3600);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 10, 86400));
//
//	setting1 = networkTimeout = group->add<QSpinBox>(NetworkTimeout, tr("Network timeout (sec)"), 10);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 1, 60));
//
//	setting1 = group->add<QSpinBox>(NetworkErrors, tr("Max network errors"), 10);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 0, 100));
//
//	setting1 = group->add<QCheckBox>(VerboseLog, tr("Verbose log"), false);
//
//
//	/* Database */
//	settings.push_back(group = new ConfigOptionGroup(tr("Database")));
//
//	setting1 = group->add<QLineEdit>(DatabaseName, tr("Data source name (DSN)"), QString::fromLatin1("CFS-Stream"));
//	constraints.push_back(new ConfigOptionConstraintEmpty(setting1));
//
//	setting1 = databaseReconnectTimeout = group->add<QSpinBox>(DatabaseUploadTimeout, tr("Upload timeout (sec)"), 30);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 10, 3600));
//
//	setting1 = group->add<QSpinBox>(DatabaseUploadRecordsQuantity, tr("Upload records quantity"), 50);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 10, 100));
//
//
//	/* WebServer */
//	settings.push_back(group = new ConfigOptionGroup(tr("WEB-Server")));
//
//	setting1 = group->add<QLineEdit>(WebServerUrl, tr("URL"), QString::fromLatin1("https://sget11.kaspersky-labs.com"));
//	constraints.push_back(new ConfigOptionConstraintUrl(setting1));
//
//	setting1 = group->add<QLineEdit>(WebServerKey, tr("Key"), QString::fromLatin1("c6ac6faea77efdb9e7f0f33aa1fc256e"));
//	constraints.push_back(new ConfigOptionConstraintEmpty(setting1));
//
//	setting1 = webServerDataRequestTimeout = group->add<QSpinBox>(WebServerDataRequestTimeout, tr("Data request timeout (sec)"), 3600);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 10, 86400));
//	constraints.push_back(new ConfigOptionConstraintLessThan(networkTimeout, webServerDataRequestTimeout));
//
//	setting1 = m_webServerUseProxy = group->add<CheckConfigOption>(WebServerUseProxy, tr("Use proxy"));
//	static_cast<CheckConfigOption*>(setting1)->setListener(this, SLOT(webServerUseProxy(int)));
//
//	subGroup = group->add(tr("Proxy"));
//	static_cast<CheckConfigOption*>(setting1)->addOption(subGroup);
//
//	setting1 = subGroup->add<QLineEdit>(WebServerProxyHost, tr("Host"), QString::fromLatin1("proxy.avp.ru"));
//	constraints.push_back(new ConfigOptionConstraintUrl(setting1));
//
//	setting1 = subGroup->add<QSpinBox>(WebServerProxyPort, tr("Port"), 3128);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 1, 65535));
//
//	setting1 = subGroup->add<QLineEdit>(WebServerProxyUser, tr("User name"), QString());
//
//	setting1 = subGroup->add<PasswordConfigOption>(WebServerProxyPass, tr("Password"));
//
//
//	/* Thumbnailer */
//	settings.push_back(group = new ConfigOptionGroup(tr("Thumbnailer")));
//
//	setting1 = group->add<QSpinBox>(ThumbnailerTimeout, tr("Timeout (sec)"), 30);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 10, 120));
//
//	setting1 = m_thumbnailerBackgroundColor = group->add<ColorConfigOption>(ThumbnailerBackgroundColor, tr("Background color"));
//	static_cast<ColorConfigOption*>(setting1)->setListener((QObject*)this, SLOT(thumbnailerBackgroundColorStateChanged(int)), SLOT(thumbnailerBackgroundColorSelect()));
//
//	setting1 = m_thumbnailerUseProxy = group->add<CheckConfigOption>(ThumbnailerUseProxy, tr("Use proxy"));
//	static_cast<CheckConfigOption*>(setting1)->setListener(this, SLOT(thumbnailerUseProxy(int)));
//
//	subGroup = group->add(tr("Proxy"));
//	static_cast<CheckConfigOption*>(setting1)->addOption(subGroup);
//
//	setting1 = subGroup->add<QLineEdit>(ThumbnailerProxyHost, tr("Host"), QString::fromLatin1("proxy.avp.ru"));
//	constraints.push_back(new ConfigOptionConstraintUrl(setting1));
//
//	setting1 = subGroup->add<QSpinBox>(ThumbnailerProxyPort, tr("Port"), 3128);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 1, 65535));
//
//	setting1 = subGroup->add<QLineEdit>(ThumbnailerProxyUser, tr("User name"), QString());
//
//	setting1 = subGroup->add<PasswordConfigOption>(ThumbnailerProxyPass, tr("Password"));
//
//
//	/* Alerts */
//	settings.push_back(group = new ConfigOptionGroup(tr("Alerts")));
//
//	setting2 = m_alertsEnabled = group->add<CheckConfigOption>(AlertsEnabled, tr("Enabled"));
//	static_cast<CheckConfigOption*>(setting2)->setListener(this, SLOT(alertsEnabled(int)));
//
//	subGroup = group->add(tr("SMTP"));
//	static_cast<CheckConfigOption*>(setting2)->addOption(subGroup);
//
//	setting1 = subGroup->add<QLineEdit>(AlertsSmtpHost, tr("Host"), QString::fromLatin1("mail.avp.ru"));
//	constraints.push_back(new ConfigOptionConstraintUrl(setting1));
//
//	setting1 = subGroup->add<QSpinBox>(AlertsSmtpPort, tr("Port"), 25);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 1, 65535));
//
//	setting1 = group->add<EmailConfigOption>(AlertsEmail, tr("Email address"));
//	static_cast<EmailConfigOption*>(setting1)->setListener((QObject*)this, SLOT(testEmailAddress()));
//	static_cast<CheckConfigOption*>(setting2)->addOption(setting1);
//
//	setting1 = group->add<QTimeEdit>(AlertsHeartbeat, tr("Heartbeat message will be sent at "), QTime());
//	static_cast<CheckConfigOption*>(setting2)->addOption(setting1);
//
//	setting1 = group->add<QSpinBox>(AlertsWebServerUnavailable, tr("WEB server unavailable during (sec)"), 7200);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 10, 5 * 3600));
//	constraints.push_back(new ConfigOptionConstraintLessThan(webServerDataRequestTimeout, setting1));
//	static_cast<CheckConfigOption*>(setting2)->addOption(setting1);
//
//	setting1 = group->add<QSpinBox>(AlertsDbServerUnavailable, tr("DB server unavailable during (sec)"), 7200);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 10, 5 * 3600));
//	constraints.push_back(new ConfigOptionConstraintLessThan(databaseReconnectTimeout, setting1));
//	static_cast<CheckConfigOption*>(setting2)->addOption(setting1);
//
//	setting1 = group->add<QSpinBox>(AlertsPhishingFtpServerErrors, tr("Max number of phishing FTP server errors"), 3);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 1, 10));
//	static_cast<CheckConfigOption*>(setting2)->addOption(setting1);
//
//	setting1 = group->add<QSpinBox>(AlertsPhishingBlackWhiteServerErrors, tr("Max number of phishing black/white list server errors"), 3);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 1, 10));
//	static_cast<CheckConfigOption*>(setting2)->addOption(setting1);
//
//	subGroup = group->add(tr("Internet"));
//	static_cast<CheckConfigOption*>(setting2)->addOption(subGroup);
//
//	setting1 = subGroup->add<QLineEdit>(AlertsInternetTestServer, tr("Test http server url"), QString::fromLatin1("http://example.com"));
//	constraints.push_back(new ConfigOptionConstraintUrl(setting1));
//
//	setting1 = setting2 = subGroup->add<QSpinBox>(AlertsInternetCheckInterval, tr("Check interval (sec)"), 3600);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 10, 5 * 3600));
//	constraints.push_back(new ConfigOptionConstraintLessThan(networkTimeout, setting1));
//
//	setting1 = subGroup->add<QSpinBox>(AlertsInternetUnavailable, tr("Unavailable during (sec)"), 7200);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 10, 5 * 3600));
//	constraints.push_back(new ConfigOptionConstraintLessThan(setting2, setting1));
//
//
//	/* Phishing */
//	settings.push_back(group = new ConfigOptionGroup(tr("Phishing")));
//
//	setting2 = m_phishingEnabled = group->add<CheckConfigOption>(PhishingEnabled, tr("Enabled"));
//	static_cast<CheckConfigOption*>(setting2)->setListener(this, SLOT(phishingEnabled(int)));
//
//	subGroup = group->add(tr("Heuristically filtering"));
//	static_cast<CheckConfigOption*>(setting2)->addOption(subGroup);
//
//	setting1 = subGroup->add<QLineEdit>(PhishingBasesFtpPath, tr("Bases ftp path"), QString::fromLatin1("ftp://ftp.avp.ru/bases/apu/apu.dat"));
//	constraints.push_back(new ConfigOptionConstraintUrl(setting1));
//
//	setting1 = subGroup->add<QSpinBox>(PhishingBasesFtpUpdateInterval, tr("Bases update interval (sec)"), 7200);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 10, 7 * 24 * 3600));
//	constraints.push_back(new ConfigOptionConstraintLessThan(networkTimeout, setting1));
//
//	subGroup = group->add(tr("Black/White listing"));
//	static_cast<CheckConfigOption*>(setting2)->addOption(subGroup);
//
//	setting1 = subGroup->add<QCheckBox>(PhishingBlackWhiteUseWhiteList, tr("Use white list"), true);
//
//	setting1 = subGroup->add<QLineEdit>(PhishingBlackWhiteDataSourceUrl, tr("Data source url"), QString::fromLatin1("http://sget9.avp.ru:8080/?action=feed"));
//	constraints.push_back(new ConfigOptionConstraintUrl(setting1));
//
//	setting1 = subGroup->add<QSpinBox>(PhishingBlackWhiteUpdateInterval, tr("Update interval (sec)"), 600);
//	constraints.push_back(new ConfigOptionConstraintRange(setting1, 10, 3600));
//	constraints.push_back(new ConfigOptionConstraintLessThan(networkTimeout, setting1));
//
//	setting1 = subGroup->add<QLineEdit>(PhishingBlackWhiteUsername, tr("User name"), QString());
//
//	setting1 = subGroup->add<PasswordConfigOption>(PhishingBlackWhitePassword, tr("Password"));


	return Argument(settings, constraints);
}

void ApplicationSettings::thumbnailerBackgroundColorSelect()
{
	QVariantList list = m_thumbnailerBackgroundColor->editorValue().toList();
	list[1] = QColorDialog::getColor(list.at(1).toString(), dialog());
	m_thumbnailerBackgroundColor->setEditorValue(list);
}

void ApplicationSettings::thumbnailerBackgroundColorStateChanged(int state)
{
	QVariantList list = m_thumbnailerBackgroundColor->editorValue().toList();
	list[0] = state == Qt::Checked;
	m_thumbnailerBackgroundColor->setEditorValue(list);
}

void ApplicationSettings::webServerUseProxy(int state)
{
	m_webServerUseProxy->setEditorValue(state == Qt::Checked);
}

void ApplicationSettings::thumbnailerUseProxy(int state)
{
	m_thumbnailerUseProxy->setEditorValue(state == Qt::Checked);
}

void ApplicationSettings::alertsEnabled(int state)
{
	m_alertsEnabled->setEditorValue(state == Qt::Checked);
}

void ApplicationSettings::phishingEnabled(int state)
{
	m_phishingEnabled->setEditorValue(state == Qt::Checked);
}
