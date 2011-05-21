#ifndef FILESYSTEMPATH_H_
#define FILESYSTEMPATH_H_

#include <QtCore/QDir>
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
		Iterator(Path &path, QStringList &list) :
			m_path(path),
			m_pos(0),
			m_list(list)
		{}

	private:
		Path &m_path;
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
		if (path.startsWith(QChar('/')))
			m_path.push_front(QString::fromLatin1("/"));
#endif
	}

	bool isValid() const { return !m_path.isEmpty(); }
	bool isAbsolute() const { return m_isAbsolute; }
	Iterator begin() { return Iterator(*this, m_path); }

private:
	bool m_isAbsolute;
	QStringList m_path;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMPATH_H_ */
