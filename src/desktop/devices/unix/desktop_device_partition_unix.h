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
#ifndef DESKTOP_DEVICE_PARTITION_UNIX_H_
#define DESKTOP_DEVICE_PARTITION_UNIX_H_

#include "../desktop_device_partition.h"


DESKTOP_NS_BEGIN

class PartitionUnix : public Partition
{
	Q_DECLARE_TR_FUNCTIONS(PartitionUnix)

public:
	PartitionUnix(const Id &id,
			  	  const QIcon &icon,
			  	  const QString &label,
			  	  bool hidden,
			  	  Drive *parent,
			  	  int number,
			  	  quint64 size,
			  	  const QStringList &mountPaths);
	virtual ~PartitionUnix();

	virtual bool mount(QString &error);
	virtual bool unmount(QString &error);

private:
	bool m_mountedByThis;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICE_PARTITION_UNIX_H_ */
