#ifndef ARCNODEDIRENTRYITEM_H_
#define ARCNODEDIRENTRYITEM_H_

#include <QtCore/QMap>
#include "arcnodeentryitem.h"


ARC_PLUGIN_NS_BEGIN

class ArcNodeDirEntryItem : public ArcNodeListItem
{
public:
	ArcNodeDirEntryItem(const QString &fileName, const QDateTime &lastModified, Base *parent = 0);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* IArcNodeItem */
	virtual bool isRoot() const;
	virtual bool isDir() const;
	virtual void lock(const QString &reason);

	const QString &fileName() const { return m_fileName; }
	size_type indexOf(const QString &fileName) const { return m_index.value(fileName, InvalidIndex); }

	void add(ArcNodeEntryItem *item)
	{
		m_index[item->fileName()] = m_items.size();
		m_items.push_back(item);

		m_extractedSize += item->extractedSize();

		for (ArcNodeDirEntryItem *p = static_cast<ArcNodeDirEntryItem *>(parent()); p; p = static_cast<ArcNodeDirEntryItem *>(p->parent()))
			p->m_extractedSize += item->extractedSize();
	}

	void add(ArcNodeDirEntryItem *item)
	{
		m_index[item->fileName()] = m_items.size();
		m_items.push_back(item);
	}

private:
	typedef QMap<QString, size_type> IndexMap;

private:
	IndexMap m_index;
	QString m_fileName;
	qint64 m_extractedSize;
	QDateTime m_lastModified;
	FileTypeInfo m_typeInfo;
};

ARC_PLUGIN_NS_END

#endif /* ARCNODEDIRENTRYITEM_H_ */
