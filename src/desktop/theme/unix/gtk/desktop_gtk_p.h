#ifndef DESKTOP_GTK_P_H_
#define DESKTOP_GTK_P_H_

#include <QtCore/QString>

#undef signals // Collides with GTK symbols
#include <gtk/gtk.h>


class GConf;
class GConfClient;

typedef GConfClient* (*Ptr_gconf_client_get_default)();
typedef char *(*Ptr_gconf_client_get_string)(GConfClient *, const char *, GError **);
typedef bool (*Ptr_gconf_client_get_bool)(GConfClient *, const char *, GError **);


class DesktopPrivate
{
public:
    DesktopPrivate();

    static bool isKDE4Session();
    static QString themeName();
    static QString iconThemeName();

private:
    static bool getGConfBool(const QString &key, bool fallback = 0);
    static QString getGConfString(const QString &key, const QString &fallback = QString());

    static Ptr_gconf_client_get_default gconf_client_get_default;
    static Ptr_gconf_client_get_string gconf_client_get_string;
    static Ptr_gconf_client_get_bool gconf_client_get_bool;
};

#endif /* DESKTOP_GTK_P_H_ */
