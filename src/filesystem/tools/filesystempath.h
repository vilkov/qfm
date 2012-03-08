#ifndef FILESYSTEMPATH_H_
#define FILESYSTEMPATH_H_

#ifdef Q_OS_WIN
#	include <QtCore/QDir>
#endif
#include <QtCore/QString>
#include <QtCore/QStringList>
#include "../filesystem_ns.h"


FILE_SYSTEM_NS_BEGIN

class Path
{
public:
	class Iterator
	{
		friend class Path;

	public:
		Iterator &operator=(const Iterator &other) { m_pos = other.m_pos; return *this; }

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
		Iterator(QStringList &list, qint32 pos = 0) :
			m_pos(pos),
			m_list(list)
		{}

	private:
		mutable qint32 m_pos;
		QStringList &m_list;
	};

public:
	Path(const QString &path) :
#ifdef Q_OS_WIN
		m_isAbsolute(path.size() >= 2 && path.at(1) == QChar(':')),
		m_path(QDir::fromNativeSeparators(path).split(QChar('/'), QString::SkipEmptyParts))
#else
		m_isAbsolute(path.startsWith(QChar('/'))),
		m_path(path.split(QChar('/'), QString::SkipEmptyParts))
#endif
	{
#ifdef Q_OS_WIN
		if (isValid())
			m_path[0].append(QChar('/')); /* TODO: This is Qt bug! */
#else
		if (m_isAbsolute)
			m_path.push_front(QString::fromLatin1("/"));
#endif
	}

	bool isValid() const { return !m_path.isEmpty(); }
	bool isAbsolute() const { return m_isAbsolute; }
	Iterator begin() { return Iterator(m_path); }
	Iterator erase(const Iterator &iterator) { m_path.removeAt(iterator.m_pos); return iterator; }
	QString toString() const { return m_path.join(QChar('/')); }

private:
	bool m_isAbsolute;
	QStringList m_path;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMPATH_H_ */
