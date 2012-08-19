#ifndef DEFAULTMIMEICONCACHE_H_
#define DEFAULTMIMEICONCACHE_H_

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

				return res;
			}
		}
	}
};

DEFAULT_PLUGIN_NS_END

#endif /* DEFAULTMIMEICONCACHE_H_ */
