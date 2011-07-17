#ifndef MOUNTPOINTS_H_
#define MOUNTPOINTS_H_

#include <QtCore/QSharedData>
#include "mountpoint.h"


class MountPoints
{
public:
	typedef MountPoint::value_type value_type;
	typedef MountPoint::size_type  size_type;

public:
	MountPoints();
	~MountPoints();

	MountPoint &operator[](size_type index);
	const MountPoint &operator[](size_type index) const;

	const MountPoint &at(size_type index) const;
	bool isEmpty() const;
	size_type size() const;

	void refresh();

private:
	class MountPointsPrivate;
	MountPointsPrivate * const m_data;
};

#endif /* MOUNTPOINTS_H_ */
