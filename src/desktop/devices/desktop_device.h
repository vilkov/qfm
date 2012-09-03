#ifndef DESKTOP_DEVICE_H_
#define DESKTOP_DEVICE_H_

#include <QtCore/QString>
#include <QtGui/QIcon>
#include "../desktop_ns.h"


DESKTOP_NS_BEGIN

class Device
{
public:
#ifdef Q_OS_UNIX
	typedef QString Id;
#else
#error Unsupported OS!
#endif

public:
	Device(const Id &id,
		   const QIcon &icon,
		   const QString &label,
		   bool hidden,
		   Device *parent);
	virtual ~Device();

    const Id &id() const { return m_id; }
	const QIcon &icon() const { return m_icon; }
	const QString &label() const { return m_label; }
	bool isHidden() const { return m_hidden; }
	Device *parent() const { return m_parent; }

	virtual bool isDrive() const;
	virtual bool isHardDrive() const;
	virtual bool isFlashDrive() const;
	virtual bool isRemovableDrive() const;
	virtual bool isOpticalDrive() const;
	virtual bool isFloppyDrive() const;
	virtual bool isPartition() const;

    template <typename R> inline
    const R *as() const { return static_cast<const R *>(this); }

    template <typename R> inline
    R *as() { return static_cast<R *>(this); }

private:
    Id m_id;
	QIcon m_icon;
	QString m_label;
	bool m_hidden;
	Device *m_parent;
};

DESKTOP_NS_END

#endif /* DESKTOP_DEVICE_H_ */
