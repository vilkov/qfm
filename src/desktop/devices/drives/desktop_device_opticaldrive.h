#ifndef DESKTOP_DEVICE_OPTICALDRIVE_H_
#define DESKTOP_DEVICE_OPTICALDRIVE_H_

#include "desktop_device_drive.h"


DESKTOP_NS_BEGIN

class OpticalDrive : public Drive
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
			  	 MediaType mediaCompatibility,
			  	 MediaType media,
			  	 bool ejectable);

	MediaType mediaCompatibility() const { return m_mediaCompatibility; }
	MediaType media() const { return m_media; }
	bool ejectable() const { return m_ejectable; }

	virtual bool isOpticalDrive() const;

private:
	MediaType m_mediaCompatibility;
	MediaType m_media;
	bool m_ejectable;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICE_OPTICALDRIVE_H_ */
