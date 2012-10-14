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
#include "desktop_kde_p.h"

#include <QtCore/QDir>
#include <QtCore/QSettings>

#include <stdlib.h>


DesktopPrivate::DesktopPrivate()
{}

QString DesktopPrivate::iconThemeName(int version)
{
    QString result = version >= 4 ? QString::fromLatin1("oxygen") : QString::fromLatin1("crystalsvg");
    QSettings settings(kdeHome(version) + QString::fromLatin1("/share/config/kdeglobals"), QSettings::IniFormat);

    settings.beginGroup(QString::fromLatin1("Icons"));
    result = settings.value(QString::fromLatin1("Theme"), result).toString();

    return result;
}

QString DesktopPrivate::kdeHome(int version)
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
