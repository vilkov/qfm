#ifndef FILESYSTEMPATH_H_
#define FILESYSTEMPATH_H_

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
	Path(const QString &path)
	{
		parseUri(path);
	}

	bool isValid() const { return !m_path.isEmpty(); }
	const QString &shema() const { return m_shema; }
	Iterator begin() { return Iterator(m_path); }
	Iterator erase(const Iterator &iterator) { m_path.removeAt(iterator.m_pos); return iterator; }
	QString toString() const { return m_path.join(QChar('/')); }

private:
	/* Simple Turing machine */

	typedef qint32                size_t;
	typedef QPair<size_t, size_t> Position;

	void parseUri(const QString &uri)
	{
		Position position(0, 0);

		for (size_t size = uri.size(); position.second < size; ++position.second)
			if (uri.at(position.second) == QChar(':'))
				shemaState(uri, position);
			else
				if (uri.at(position.second) == QChar('/'))
					pathState(uri, position);

		if (position.first < uri.size())
		{
			m_path.push_back(uri.mid(position.first, uri.size() - position.first));

#ifndef Q_OS_WIN32
			if (m_path.at(0).isEmpty())
				m_path[0] = QChar('/');
		}
		else
			if (!m_path.isEmpty() && m_path.at(0).isEmpty())
				m_path[0] = QChar('/');
#else
		}
#endif
	}

	void shemaState(const QString &uri, Position &position)
	{
		if (++position.second < uri.size() && uri.at(position.second) == QChar('/'))
			if (++position.second < uri.size() && uri.at(position.second) == QChar('/'))
				if (m_shema.isEmpty())
				{
					m_shema = uri.mid(position.first, position.second - position.first - 2);
					position.first = position.second + 1;
				}
				else
				{
					m_path.push_back(uri.mid(position.first, position.second - position.first - 1));
					position.first = position.second + 1;
				}
			else
			{
				m_path.push_back(uri.mid(position.first, position.second - position.first - 1));
				position.first = position.second + 1;
			}
	}

	void pathState(const QString &uri, Position &position)
	{
		m_path.push_back(uri.mid(position.first, position.second - position.first));
		position.first = position.second + 1;
	}

private:
	QString m_shema;
	QStringList m_path;
};

FILE_SYSTEM_NS_END

#endif /* FILESYSTEMPATH_H_ */
