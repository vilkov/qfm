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
#ifndef VFS_URI_H_
#define VFS_URI_H_

#include <QtCore/QPair>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include "../vfs_ns.h"


VFS_NS_BEGIN

class Uri
{
public:
	class iterator
	{
		friend class Uri;

	public:
		iterator &operator=(const iterator &other) { m_pos = other.m_pos; return *this; }

		const iterator &operator++() const { ++m_pos; return *this; }
		iterator &operator++() { ++m_pos; return *this; }

		const iterator &operator++(int) const { ++m_pos; return *this; }
		iterator &operator++(int) { ++m_pos; return *this; }

		const iterator &operator--() const { --m_pos; return *this; }
		iterator &operator--() { --m_pos; return *this; }

		const iterator &operator--(int) const { --m_pos; return *this; }
		iterator &operator--(int) { --m_pos; return *this; }

		const QString &operator*() const { return m_list[m_pos]; }

		bool atEnd() const { return m_pos == m_list.size(); }
        bool isLast() const { return m_pos == m_list.size() - 1; }

	private:
		iterator(const QStringList &list, qint32 pos = 0) :
			m_pos(pos),
			m_list(list)
		{}

	private:
		mutable qint32 m_pos;
		const QStringList &m_list;
	};

public:
	Uri(const QString &path);
	Uri(const QByteArray &path);

	bool isValid() const { return m_valid; }
	bool isEmpty() const { return m_path.isEmpty(); }

	const QString &schema() const { return m_schema; }
	const QString &userName() const { return m_userName; }
	const QString &password() const { return m_password; }
	ushort port() const { return m_port; }
	const QString &domain() const { return m_domain; }

	iterator begin() const { return iterator(m_path); }
	iterator erase(const iterator &iterator) { m_path.removeAt(iterator.m_pos); return iterator; }
	QString toString() const { return m_path.join(QChar(L'/')); }

private:
	bool m_valid;
	QString m_schema;
	QString m_userName;
	QString m_password;
	ushort m_port;
	QString m_domain;
	QStringList m_path;
};

VFS_NS_END

#endif /* VFS_URI_H_ */
