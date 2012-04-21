#include "mountpoints.h"

#include <stlsoft/stlsoft.h>
#if defined(STLSOFT_CF_EXCEPTION_SUPPORT)
#	undef STLSOFT_CF_EXCEPTION_SUPPORT
#endif
#include <platformstl/platformstl.hpp>

#if defined(PLATFORMSTL_OS_IS_WINDOWS)
#	include "win32/mountpoints_p.h"
#elif defined(PLATFORMSTL_OS_IS_UNIX)
#	include "unix/mountpoints_p.h"
#else
#	error OS is unknown!
#endif


MountPoints::MountPoints() :
	m_data(new MountPointsPrivate())
{}

MountPoints::~MountPoints()
{
	delete m_data;
}

MountPoint &MountPoints::operator[](size_type index)
{
	return m_data->items[index];
}

const MountPoint &MountPoints::operator[](size_type index) const
{
	return m_data->items[index];
}

const MountPoint &MountPoints::at(size_type index) const
{
	return m_data->items.at(index);
}

bool MountPoints::isEmpty() const
{
	return m_data->items.isEmpty();
}

MountPoints::size_type MountPoints::size() const
{
	return m_data->items.size();
}

void MountPoints::refresh()
{
	m_data->refresh();
}
