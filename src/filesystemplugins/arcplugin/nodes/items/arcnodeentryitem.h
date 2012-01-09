#ifndef ARCNODEENTRYITEM_H_
#define ARCNODEENTRYITEM_H_

#include <QtCore/QMap>
#include <QtCore/QString>
#include "arcnodeitem.h"


ARC_PLUGIN_NS_BEGIN

class ArcNodeEntryItem : public ArcNodeListItem
{
public:
	ArcNodeEntryItem(const QString &fileName, Base *parent = 0);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	bool isDir() const { return m_fileName.endsWith(QChar('/')); }
	size_type indexOf(const QString &fileName) const { return m_index.value(fileName, InvalidIndex); }

private:
	friend class Archive;
	void add(ArcNodeEntryItem *item)
	{
		m_index[item->m_fileName] = m_items.size();
		m_items.push_back(item);
	}

private:
	typedef QMap<QString, size_type> IndexMap;

private:
	IndexMap m_index;
	QString m_fileName;
};

ARC_PLUGIN_NS_END

#endif /* ARCNODEENTRYITEM_H_ */
