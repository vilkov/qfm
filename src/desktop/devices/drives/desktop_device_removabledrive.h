#ifndef DESKTOP_DEVICE_REMOVABLEDRIVE_H_
#define DESKTOP_DEVICE_REMOVABLEDRIVE_H_

#include "desktop_device_drive.h"


DESKTOP_NS_BEGIN

class RemovableDrive : public Drive
{
public:
	struct Details : public Drive::Details
	{
		bool writeCacheEnabled;
		bool canDetach;
		bool canSpindown;
	};

public:
	RemovableDrive(const Id &id,
			  	   const QIcon &icon,
			  	   const QString &label,
			  	   bool hidden,
			  	   Device *parent,
			  	   quint64 size,
			   	   MediaTypeSet mediaCompatibility,
			   	   MediaType media,
			   	   bool ejectable);

	const MediaTypeSet &mediaCompatibility() const { return m_mediaCompatibility; }

	MediaType media() const { return m_media; }
	void setMedia(MediaType media) { m_media = media; }

	bool ejectable() const { return m_ejectable; }
	void setEjectable(bool ejectable) { m_ejectable = ejectable; }

	virtual bool isRemovableDrive() const;

private:
	MediaTypeSet m_mediaCompatibility;
	MediaType m_media;
	bool m_ejectable;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICE_REMOVABLEDRIVE_H_ */
