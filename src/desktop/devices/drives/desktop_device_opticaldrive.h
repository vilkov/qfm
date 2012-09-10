#ifndef DESKTOP_DEVICE_OPTICALDRIVE_H_
#define DESKTOP_DEVICE_OPTICALDRIVE_H_

#include "desktop_device_removabledrive.h"


DESKTOP_NS_BEGIN

class OpticalDrive : public RemovableDrive
{
public:
	struct Details : public Drive::Details
	{
		bool writeCacheEnabled;
		bool canDetach;
		bool canSpindown;
		bool isBlank;
		bool isAppendable;
		bool isClosed;
		quint32 numTracks;
		quint32 numAudioTracks;
		quint32 numSessions;
	};

public:
	OpticalDrive(const Id &id,
			  	 const QIcon &icon,
			  	 const QString &label,
			  	 bool hidden,
			  	 Device *parent,
			  	 quint64 size,
			  	 MediaTypeSet mediaCompatibility,
			  	 MediaType media,
			  	 bool detachable,
			  	 bool ejectable);

	bool isEjectable() const { return m_ejectable; }
	void setEjectable(bool ejectable) { m_ejectable = ejectable; }

	static const MediaTypeSet &mediaTypeSet();
	virtual bool isOpticalDrive() const;

private:
	bool m_ejectable;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICE_OPTICALDRIVE_H_ */
