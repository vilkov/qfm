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
