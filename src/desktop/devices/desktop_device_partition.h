#ifndef DESKTOP_DEVICE_PARTITION_H_
#define DESKTOP_DEVICE_PARTITION_H_

#include <QtCore/QStringList>
#include "desktop_device.h"


DESKTOP_NS_BEGIN
class Drive;


class Partition : public Device
{
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

	bool mount(QString &error);

private:
	int m_number;
	quint64 m_size;
	QStringList m_mountPaths;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICE_PARTITION_H_ */
