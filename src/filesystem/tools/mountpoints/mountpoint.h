#ifndef MOUNTPOINT_H_
#define MOUNTPOINT_H_

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>


class MountPoint
{
public:
	typedef QList<MountPoint>     value_type;
	typedef value_type::size_type size_type;

public:
	MountPoint(const QString &path, const QString &label, const QString &systemPath, const QPixmap &pixmap, quint64 freeSize, quint64 totalSize) :
		m_path(path),
		m_label(label),
		m_systemPath(systemPath),
		m_icon(pixmap),
		m_freeSize(freeSize),
		m_totalSize(totalSize)
	{}

	const QString &path() const { return m_path; }
	const QString &label() const { return m_label; }
	const QString &systemPath() const { return m_systemPath; }
	const QIcon &icon() const { return m_icon; }
	quint64 freeSize() const { return m_freeSize; }
	quint64 totalSize() const { return m_totalSize; }

private:
	QString m_path;
	QString m_label;
	QString m_systemPath;
	QIcon m_icon;
	quint64 m_freeSize;
	quint64 m_totalSize;
};

#endif /* MOUNTPOINT_H_ */
