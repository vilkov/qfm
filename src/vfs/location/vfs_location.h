#ifndef VFS_LOCATION_H_
#define VFS_LOCATION_H_

#include <QtCore/QString>
#include <QtCore/QByteArray>
#include "../vfs_ns.h"


VFS_NS_BEGIN

class Location
{
public:
	Location()
	{}

	operator const QString &() const { return m_label; }
	operator const QByteArray &() const { return m_location; }

	bool isValid() const { return !m_location.isNull(); }

	template <typename R> inline
	const R &as() const;

protected:
	friend class IFileContainer;
	Location(const QString &label, const QByteArray &location) :
		m_label(label),
		m_location(location)
	{}

private:
	QString m_label;
	QByteArray m_location;
};

template <> inline
const QString &Location::as() const
{
	return m_label;
}

template <> inline
const QByteArray &Location::as() const
{
	return m_location;
}

VFS_NS_END

#endif /* VFS_LOCATION_H_ */
