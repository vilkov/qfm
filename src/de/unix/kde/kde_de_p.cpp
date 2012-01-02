#include "kde_de_p.h"
#include <QtCore/QDir>
#include <QtCore/QSettings>
#include <stdlib.h>


DesktopEnvironmentPrivate::DesktopEnvironmentPrivate()
{}

QString DesktopEnvironmentPrivate::iconThemeName(int version)
{
    QString result = version >= 4 ? QString::fromLatin1("oxygen") : QString::fromLatin1("crystalsvg");
    QSettings settings(kdeHome(version) + QString::fromLatin1("/share/config/kdeglobals"), QSettings::IniFormat);

    settings.beginGroup(QString::fromLatin1("Icons"));
    result = settings.value(QString::fromLatin1("Theme"), result).toString();

    return result;
}

QString DesktopEnvironmentPrivate::kdeHome(int version)
{
    static QString kdeHomePath;

    if (kdeHomePath.isEmpty())
    {
    	if (const char *var = getenv("KDEHOME"))
    		kdeHomePath = QString::fromLocal8Bit(var);

        if (kdeHomePath.isEmpty())
        {
            QDir homeDir(QDir::homePath());
            QString kdeConfDir(QString::fromLatin1("/.kde"));

            if (version == 4 && homeDir.exists(QString::fromLatin1(".kde4")))
            	kdeConfDir = QString::fromLatin1("/.kde4");

            kdeHomePath = QDir::homePath() + kdeConfDir;
        }
    }

    return kdeHomePath;
}
