#ifndef FILESYSTEMPATH_H_
#define FILESYSTEMPATH_H_

#include <QtCore/QString>
#include <QtCore/QStringList>
#include "filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class Path
{
public:
	class Iterator
	{
		friend class Path;

	public:
		const Iterator &operator++() const { ++m_pos; return *this; }
		Iterator &operator++() { ++m_pos; return *this; }

		const Iterator &operator++(int) const { ++m_pos; return *this; }
		Iterator &operator++(int) { ++m_pos; return *this; }

		const Iterator &operator--() const { --m_pos; return *this; }
		Iterator &operator--() { --m_pos; return *this; }

		const Iterator &operator--(int) const { --m_pos; return *this; }
		Iterator &operator--(int) { --m_pos; return *this; }

		QString &operator*() { return m_list[m_pos]; }
		const QString &operator*() const { return m_list[m_pos]; }

		bool atEnd() const { return m_pos == m_list.size(); }

	private:
		Iterator(QStringList &list) :
			m_pos(0),
			m_list(list)
		{}

	private:
		mutable qint32 m_pos;
		QStringList &m_list;
	};

public:
	Path(const QString &path) :
		m_path(path.split(QChar('/'), QString::SkipEmptyParts))
	{}

	bool isValid() const { return !m_path.isEmpty(); }
	Iterator start() { return Iterator(m_path); }

private:
	QStringList m_path;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMPATH_H_ */
