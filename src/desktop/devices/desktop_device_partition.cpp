#include "desktop_device_partition.h"
#include "drives/desktop_device_drive.h"


DESKTOP_NS_BEGIN

Partition::Partition(const Id &id,
		  	  	  	 const QIcon &icon,
		  	  	  	 const QString &label,
		  	  	  	 bool hidden,
		  	  	  	 Drive *parent,
		  	  	  	 int number,
		  	  	  	 quint64 size,
		  	  	  	 const QStringList &mountPaths) :
	Device(id, icon, label, hidden, parent),
	m_mountPaths(mountPaths),
	m_number(number),
	m_size(size)
{}

bool Partition::isPartition() const
{
	return true;
}

bool Partition::mount(QString &error)
{
	error = tr("Not implemented");
	return false;
}

bool Partition::unmount(QString &error)
{
	error = tr("Not implemented");
	return false;
}

DESKTOP_NS_END
