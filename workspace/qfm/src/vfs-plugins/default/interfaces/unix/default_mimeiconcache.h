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
#ifndef DEFAULT_MIMEICONCACHE_H_
#define DEFAULT_MIMEICONCACHE_H_

#include <desktop/theme/unix/desktop_iconcache.h>
#include "../../default_ns.h"


DEFAULT_PLUGIN_NS_BEGIN

class MimeIconCache : protected ::Desktop::IconCache
{
public:
	MimeIconCache() :
		::Desktop::IconCache()
	{}

	QIcon findIcon(const char *mimeType, int iconSize, const QByteArray &theme)
	{
		Index index(mimeType, iconSize, XdgThemeMimeTypes);

		if (QIcon *icon = lockedRead(index))
			return *icon;
		else
		{
			QWriteLocker lock(&m_cacheLock);

			if (icon = read(index))
				return *icon;
			else
			{
				QIcon res;

				if (char *icon_path = xdg_mime_type_icon_lookup(mimeType, iconSize, theme))
				{
					res = *write(index, QString::fromUtf8(icon_path));
					free(icon_path);
				}
				else
					write(index);

				return res;
			}
		}
	}

	QIcon findIcon(const char *mimeType, const char *fallbackMimeType, int iconSize, const QByteArray &theme)
	{
		Index index(mimeType, iconSize, XdgThemeMimeTypes);

		if (QIcon *icon = lockedRead(index))
			return *icon;
		else
		{
			QWriteLocker lock(&m_cacheLock);

			if (icon = read(index))
				return *icon;
			else
			{
				QIcon res;

				if (char *icon_path = xdg_mime_type_icon_lookup(mimeType, iconSize, theme))
				{
					res = *write(index, QString::fromUtf8(icon_path));
					free(icon_path);
				}
				else
					if (icon_path = xdg_mime_type_icon_lookup(fallbackMimeType, iconSize, theme))
					{
						res = *write(index, QString::fromUtf8(icon_path));
						free(icon_path);
					}
					else
						write(index);

				return res;
			}
		}
	}
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULT_MIMEICONCACHE_H_ */
