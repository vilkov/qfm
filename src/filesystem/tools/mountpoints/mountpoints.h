#ifndef MOUNTPOINTS_H_
#define MOUNTPOINTS_H_

#include <QtCore/QList>
#include <QtCore/QString>
#include <QtGui/QIcon>
#include <QtGui/QPixmap>


class MountPoints
{
public:
	class MountPoint
	{
	public:
		MountPoint(const QString &path, const QString &label, const QPixmap &pixmap, quint64 freeSize, quint64 totalSize) :
			m_path(path),
			m_label(label),
			m_icon(pixmap),
			m_freeSize(freeSize),
			m_totalSize(totalSize)
		{}

		const QString &path() const { return m_path; }
		const QString &label() const { return m_label; }
		const QIcon &icon() const { return m_icon; }
		quint64 freeSize() const { return m_freeSize; }
		quint64 totalSize() const { return m_totalSize; }

	private:
		QString m_path;
		QString m_label;
		QIcon m_icon;
		quint64 m_freeSize;
		quint64 m_totalSize;
	};

	typedef QList<MountPoint>     value_type;
	typedef value_type::size_type size_type;

public:
	MountPoints();

	MountPoint &operator[](size_type index) { return m_items[index]; }
	const MountPoint &operator[](size_type index) const { return m_items[index]; }

	const MountPoint &at(size_type index) const { return m_items.at(index); }
	bool isEmpty() const { return m_items.isEmpty(); }
	size_type size() const { return m_items.size(); }

	void refresh();

private:
	value_type m_items;
};

#endif /* MOUNTPOINTS_H_ */
