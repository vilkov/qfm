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
#include "idm_entity.h"


IDM_PLUGIN_NS_BEGIN

Entity::Entity(Type type, id_type id,
		const QString &name,
		const ShortFormat &shortFormat,
		const QByteArray &editorGeometry,
		const QByteArray &listGeometry) :
	m_type(type),
	m_id(id),
	m_name(name),
	m_shortFormat(shortFormat),
	m_editorGeometry(editorGeometry.isEmpty() ? QRect() : geometryFromByteArray(editorGeometry)),
	m_listGeometry(listGeometry.isEmpty() ? QRect() : geometryFromByteArray(listGeometry))
{}

Entity::~Entity()
{}

QByteArray Entity::geometryToByteArray(const QRect &geometry)
{
	QByteArray buffer(sizeof(int) * 4, Qt::Uninitialized);
	int *data = (int *)buffer.data();

	data[0] = geometry.left();
	data[1] = geometry.top();
	data[2] = geometry.right();
	data[3] = geometry.bottom();

	return buffer;
}

QRect Entity::geometryFromByteArray(const QByteArray &buffer)
{
	const int *data = (const int *)buffer.data();
	return QRect(QPoint(data[0], data[1]), QPoint(data[2], data[3]));
}

IDM_PLUGIN_NS_END
