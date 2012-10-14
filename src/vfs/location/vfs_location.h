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
#ifndef VFS_LOCATION_H_
#define VFS_LOCATION_H_

#include <QtCore/QString>
#include <QtCore/QByteArray>
#include "../vfs_ns.h"


VFS_NS_BEGIN

class Location
{
public:
	Location()
	{}

	operator const QString &() const { return m_label; }
	operator const QByteArray &() const { return m_location; }

	bool operator<(const Location &other) const { return m_location < other.m_location; }
	bool operator==(const Location &other) const { return m_location == other.m_location; }

	bool isValid() const { return !m_location.isNull(); }

	template <typename R> inline
	const R &as() const;

protected:
	friend class IFileInfo;
	friend class IFileContainer;
	Location(const QString &label, const QByteArray &location) :
		m_label(label),
		m_location(location)
	{}

private:
	QString m_label;
	QByteArray m_location;
};

template <> inline
const QString &Location::as() const
{
	return m_label;
}

template <> inline
const QByteArray &Location::as() const
{
	return m_location;
}

VFS_NS_END

#endif /* VFS_LOCATION_H_ */
