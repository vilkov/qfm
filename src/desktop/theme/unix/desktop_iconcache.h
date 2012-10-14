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
#ifndef DESKTOP_ICONCACHE_H_
#define DESKTOP_ICONCACHE_H_

#include <xdg/xdg.h>
#include <QtGui/QIcon>
#include <QtCore/QMap>
#include <QtCore/QByteArray>
#include <QtCore/QReadWriteLock>
#include "../../desktop_ns.h"


DESKTOP_NS_BEGIN

class IconCache
{
protected:
	struct Index
	{
		Index() :
			size(0),
			context(XdgThemeActions)
		{}

		Index(const char *name, int size, Context context) :
			name(name),
			size(size),
			context(context)
		{}

		bool operator==(const Index &other) const
		{
			return context == other.context && size == other.size && name == other.name;
		}

		bool operator<(const Index &other) const
		{
			return context < other.context || size < other.size || name < other.name;
		}

		QByteArray name;
		int size;
		Context context;
	};

public:
	IconCache()
	{}

	~IconCache()
	{
		qDeleteAll(m_cache);
	}

	QIcon findIcon(const char *name, int iconSize, Context context, const QByteArray &theme)
	{
		Index index(name, iconSize, context);

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

				if (char *icon_path = ::xdg_icon_lookup(name, iconSize, context, theme))
				{
					res = *write(index, QString::fromUtf8(icon_path));
					::free(icon_path);
				}
				else
					write(index);

				return res;
			}
		}
	}

protected:
	QIcon *read(const Index &index)
	{
		return m_cache.value(index);
	}

	QIcon *lockedRead(const Index &index)
	{
		QReadLocker lock(&m_cacheLock);
		return read(index);
	}

	void write(const Index &index)
	{
		m_cache.insert(index, new QIcon());
	}

	QIcon *write(const Index &index, const QString &fileName)
	{
		QScopedPointer<QIcon> icon(new QIcon());

		icon->addFile(fileName, QSize(index.size, index.size));
		m_cache.insert(index, icon.data());

		return icon.take();
	}

protected:
	QReadWriteLock m_cacheLock;

private:
	QMap<Index, QIcon *> m_cache;
};

DESKTOP_NS_END

#endif /* DESKTOP_ICONCACHE_H_ */
