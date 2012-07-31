#ifndef VFS_URI_H_
#define VFS_URI_H_

#include <QtCore/QPair>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include "../vfs_ns.h"


VFS_NS_BEGIN

class Uri
{
public:
	class Iterator
	{
		friend class Uri;

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
	Uri(const QString &path);
	Uri(const QByteArray &path);

	bool isValid() const { return m_valid; }

	const QString &shema() const { return m_shema; }
	const QString &userName() const { return m_userName; }
	const QString &password() const { return m_password; }
	ushort port() const { return m_port; }
	const QString &domain() const { return m_domain; }

	Iterator begin() { return Iterator(m_path); }
	Iterator erase(const Iterator &iterator) { m_path.removeAt(iterator.m_pos); return iterator; }
	QString toString() const { return m_path.join(QChar(L'/')); }

private:
	bool m_valid;
	QString m_shema;
	QString m_userName;
	QString m_password;
	ushort m_port;
	QString m_domain;
	QStringList m_path;
};

VFS_NS_END

#endif /* VFS_URI_H_ */
