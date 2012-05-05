#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <QtCore/QMap>
#include <QtCore/QList>
#include <QtCore/QString>
#include <QtCore/QVariant>
#include <QtCore/QSettings>
#include "settings_ns.h"
#include "configoptions/configoptiongroup.h"
#include "constraints/configoptionconstraint.h"
#include "../templates/metatemplates.h"


SETTINGS_NS_BEGIN

class Settings
{
public:
    typedef QList<ConfigOptionGroup*>            SettingsList;
    typedef QList<ConfigOptionConstraint*>       ConstraintsList;
    typedef QPair<SettingsList, ConstraintsList> Argument;

public:
	Settings(const QString &title, const QString &appStorage, const Argument &arg);
	virtual ~Settings();

	QVariant readValue(const qint32 &settingId);
    void exec(QWidget *parent = 0);

protected:
	QWidget *dialog() const { return m_dialog; }
	QVariant readValueFromEditor(const qint32 &settingId) const;

	void beginGroup(const QString &group) { m_storage.beginGroup(group); }
	QVariant value(const QString &setting) { return m_storage.value(setting); }
	void setValue(const QString &setting, const QVariant &value) { m_storage.setValue(setting, value); }
	void endGroup() { m_storage.endGroup(); }

    static QString storageLocation(const QString &applicationFolder);

private:
	template <typename T>
	T readValue(const QString &group, const QString &setting, const T &defaultValue)
	{
		QVariant value;
		m_storage.beginGroup(group);

		if ((value = m_storage.value(setting)).isNull())
			m_storage.setValue(setting, value = defaultValue);

		m_storage.endGroup();
		return qVariantValue<T>(value);
	}

private:
	typedef QMap<ConfigOptionGroup*, QString> GroupPathCache;

private:
	void makeSettingsCache();
	void makeSettingsCache(const QString &groupPath, GroupPathCache &groupPathCache, ConfigOptionGroup *group);
	void readSettingsValues();
	void readSettingsValues(const QString &path, ConfigOptionGroup *group);
	void writeSettingsValues();
	void writeSettingsValues(const QString &path, ConfigOptionGroup *group);

private:
	struct CacheEntry
	{
		CacheEntry() :
			option(0)
		{}

		CacheEntry(ConfigOptionValue *option, const QString &groupPath) :
			option(option),
			groupPath(groupPath)
		{}

		ConfigOptionValue *option;
		QString groupPath;
	};
	typedef QMap<qint32, CacheEntry> Cache;

private:
	QWidget *m_dialog;
	QString m_title;
	QSettings m_storage;
	SettingsList m_settings;
	ConstraintsList m_constraints;
	Cache m_cache;
};

SETTINGS_NS_END

#endif /* SETTINGS_H_ */
