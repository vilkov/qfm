#include "desktop_gtk_p.h"

#include <QtCore/QFile>
#include <QtCore/QLibrary>
#include <QtCore/QTextStream>

#include <stdlib.h>


Ptr_gconf_client_get_default DesktopPrivate::gconf_client_get_default = 0;
Ptr_gconf_client_get_string DesktopPrivate::gconf_client_get_string = 0;
Ptr_gconf_client_get_bool DesktopPrivate::gconf_client_get_bool = 0;


static bool resolveGConf()
{
    if (!DesktopPrivate::gconf_client_get_default)
    {
        DesktopPrivate::gconf_client_get_default = (Ptr_gconf_client_get_default)QLibrary::resolve(QString::fromLatin1("gconf-2"), 4, "gconf_client_get_default");
        DesktopPrivate::gconf_client_get_string =  (Ptr_gconf_client_get_string)QLibrary::resolve(QString::fromLatin1("gconf-2"), 4, "gconf_client_get_string");
        DesktopPrivate::gconf_client_get_bool =  (Ptr_gconf_client_get_bool)QLibrary::resolve(QString::fromLatin1("gconf-2"), 4, "gconf_client_get_bool");
    }

    return (DesktopPrivate::gconf_client_get_default != 0);
}


DesktopPrivate::DesktopPrivate()
{}

bool DesktopPrivate::isKDE4Session()
{
    static int version = -1;

    if (version == -1)
    	if (const char *var = getenv("KDE_SESSION_VERSION"))
    		version = atoi(var);

    return (version == 4);
}

QString DesktopPrivate::themeName()
{
    /**
     * We try to parse the gtkrc file first
     * primarily to avoid resolving Gtk functions if
     * the KDE 3 "Qt" style is currently in use
     *
     */

	QString themeName;
    QString rcPaths = QString::fromLocal8Bit(getenv("GTK2_RC_FILES"));

    if (!rcPaths.isEmpty())
    {
        QStringList paths = rcPaths.split(QString::fromLatin1(":"));

        foreach (const QString &rcPath, paths)
        {
            if (!rcPath.isEmpty())
            {
                QFile rcFile(rcPath);

                if (rcFile.exists() && rcFile.open(QIODevice::ReadOnly | QIODevice::Text))
                {
                    QTextStream in(&rcFile);

                    while(!in.atEnd())
                    {
                        QString line = in.readLine();

                        if (line.contains(QString::fromLatin1("gtk-theme-name")))
                        {
                            line = line.right(line.length() - line.indexOf(QLatin1Char('=')) - 1);
                            line.remove(QLatin1Char('\"'));
                            line = line.trimmed();
                            themeName = line;
                            break;
                        }
                    }
                }
            }

            if (!themeName.isEmpty())
                break;
        }
    }

    if (themeName.isEmpty() && resolveGConf())
        themeName = getGConfString(QString::fromLatin1("/desktop/gnome/interface/gtk_theme"));

    return themeName;
}

QString DesktopPrivate::iconThemeName()
{
    return getGConfString(QString::fromLatin1("/desktop/gnome/interface/icon_theme"), QString::fromLatin1("gnome"));
}

bool DesktopPrivate::getGConfBool(const QString &key, bool fallback)
{
    bool retVal = fallback;

    if (resolveGConf())
    {
        g_type_init();

        GError *err = 0;
        GConfClient* client = gconf_client_get_default();
        bool result = gconf_client_get_bool(client, key.toUtf8().constData(), &err);

        g_object_unref(client);

        if (!err)
            retVal = result;
        else
            g_error_free(err);
    }

    return retVal;
}

QString DesktopPrivate::getGConfString(const QString &value, const QString &fallback)
{
    QString retVal = fallback;

    if (resolveGConf())
    {
        g_type_init();

        GError *err = 0;
        GConfClient* client = gconf_client_get_default();
        char *str = gconf_client_get_string(client, value.toUtf8().constData(), &err);

        if (!err)
        {
            retVal = QString::fromUtf8(str);
            g_free(str);
        }

        g_object_unref(client);

        if (err)
            g_error_free(err);
    }

    return retVal;
}
