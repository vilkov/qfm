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
#ifndef DESKTOP_DEVICE_PARTITION_H_
#define DESKTOP_DEVICE_PARTITION_H_

#include <QtCore/QStringList>
#include <QtCore/QCoreApplication>
#include "desktop_device.h"


DESKTOP_NS_BEGIN
class Drive;


class Partition : public Device
{
	Q_DECLARE_TR_FUNCTIONS(Partition)

public:
	Partition(const Id &id,
			  const QIcon &icon,
			  const QString &label,
			  bool hidden,
			  Drive *parent,
			  int number,
			  quint64 size,
			  const QStringList &mountPaths);

	int number() const { return m_number; }
	void setNumber(int number) { m_number = number; }

	quint64 size() const { return m_size; }
	void setSize(quint64 size) { m_size = size; }

	const QStringList &mountPaths() const { return m_mountPaths; }
	void setMountPaths(const QStringList &mountPaths) { m_mountPaths = mountPaths; }

	virtual bool isPartition() const;
	virtual bool mount(QString &error);
	virtual bool unmount(QString &error);

protected:
	QStringList m_mountPaths;

private:
	int m_number;
	quint64 m_size;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICE_PARTITION_H_ */
