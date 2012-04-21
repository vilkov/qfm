#ifndef KSTUBS_H_
#define KSTUBS_H_

#include <QtCore/QList>
#include <QtCore/QString>

struct KFileItem
{
public:
	KFileItem()
	{}
	KFileItem(const QString &string) :
		m_string(string)
	{}

	bool operator==(const KFileItem &other) const { return m_string == other.m_string; }

	QString m_string;

	bool isNull() const { return m_string.isNull(); }
	QString url() const { return m_string; }
	QString text() const { return m_string; }
};

uint qHash(const KFileItem &item);

typedef QList<KFileItem> KFileItemList;


typedef QString KUrl;

#endif /* KSTUBS_H_ */
