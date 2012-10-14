/**
 * This file is part of QFM.
 *
 * Copyright (C) 2011-2012 Dmitriy Vilkov, <dav.daemon@gmail.com>
 *
 * QFM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * QFM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with QFM. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef APPLICATIONSETTINGS_H_
#define APPLICATIONSETTINGS_H_

#include <QtCore/QMutex>
#include <QtCore/QAbstractItemModel>
#include <QtCore/QCoreApplication>
#include <QtGui/QColor>
#include <QtGui/QSplitter>
#include <QtGui/QAbstractItemView>
#include "../tools/settings/settings.h"


class ApplicationSettings : public QObject, public ::Tools::Settings::Settings
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
	::Tools::Settings::ConfigOptionValue *m_thumbnailerBackgroundColor;
	::Tools::Settings::ConfigOptionValue *m_webServerUseProxy;
	::Tools::Settings::ConfigOptionValue *m_thumbnailerUseProxy;
	::Tools::Settings::ConfigOptionValue *m_alertsEnabled;
	::Tools::Settings::ConfigOptionValue *m_phishingEnabled;
	QMutex m_mutex;
};

#endif /* APPLICATIONSETTINGS_H_ */
