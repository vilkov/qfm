#ifndef APPLICATIONSETTINGS_H_
#define APPLICATIONSETTINGS_H_

#include <QMutex>
#include <QColor>
#include <QSplitter>
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QCoreApplication>
#include "../tools/settings/settings.h"


class ApplicationSettings : public QObject, public Settings
{
	Q_OBJECT

public:
	enum SettingId
	{
		/* Common */
		CommonBase = 100,
		StorageLocation = CommonBase + 1,
		NumberOfThreads = CommonBase + 2,
		TasksPerThread = CommonBase + 3,
		KeepCacheRecordsInMemory = CommonBase + 4,
		IdentityCacheRecordsLifeTime = CommonBase + 5,
		NetworkTimeout = CommonBase + 6,
		NetworkErrors = CommonBase + 7,
		VerboseLog = CommonBase + 8
	};

public:
	ApplicationSettings(QObject *parent = 0);

	QVariant readValue(const qint32 &settingId);

	void saveState(QWidget *widget);
	void saveState(QWidget *widget, QAbstractItemView *view);
	void saveState(QWidget *widget, QSplitter *splitter, QAbstractItemView *view);
	void loadState(QWidget *widget);
	void loadState(QWidget *widget, QAbstractItemView *view);
	void loadState(QWidget *widget, QSplitter *splitter, QAbstractItemView *view);

    static QString storageLocation();

private Q_SLOTS:
	void thumbnailerBackgroundColorSelect();
	void thumbnailerBackgroundColorStateChanged(int state);
	void webServerUseProxy(int state);
	void thumbnailerUseProxy(int state);
	void alertsEnabled(int state);
	void phishingEnabled(int state);

private:
	Argument createSettins();

private:
	ConfigOptionValue *m_thumbnailerBackgroundColor;
	ConfigOptionValue *m_webServerUseProxy;
	ConfigOptionValue *m_thumbnailerUseProxy;
	ConfigOptionValue *m_alertsEnabled;
	ConfigOptionValue *m_phishingEnabled;
	QMutex m_mutex;
};

#endif /* APPLICATIONSETTINGS_H_ */
