#ifndef DESKTOP_DEVICE_HARDDRIVE_H_
#define DESKTOP_DEVICE_HARDDRIVE_H_

#include "desktop_device_drive.h"


DESKTOP_NS_BEGIN

class HardDrive : public Drive
{
public:
	struct Details : public Drive::Details
	{
		quint16 rotationRate;
		bool writeCacheEnabled;
		bool canDetach;
		bool canSpindown;
		bool ataSmartIsAvailable;
		bool ataSmartTimeCollected;
		bool ataSmartStatus;
		QByteArray ataSmartBlob;
	};

public:
	HardDrive(const Id &id,
			  const QIcon &icon,
			  const QString &label,
			  bool hidden,
			  Device *parent,
			  quint64 size);

	virtual bool isHardDrive() const;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICE_HARDDRIVE_H_ */
