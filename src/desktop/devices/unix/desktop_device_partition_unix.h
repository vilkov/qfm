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
