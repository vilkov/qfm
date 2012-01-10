#ifndef ARCNODEDIRENTRYITEM_H_
#define ARCNODEDIRENTRYITEM_H_

#include <QtCore/QMap>
#include "arcnodeentryitem.h"


ARC_PLUGIN_NS_BEGIN

class ArcNodeDirEntryItem : public ArcNodeListItem
{
public:
	ArcNodeDirEntryItem(const QString &fileName, Base *parent = 0);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	/* IArcNodeItem */
	virtual bool isRoot();
	virtual bool isDir();

	const QString &fileName() const { return m_fileName; }
	size_type indexOf(const QString &fileName) const { return m_index.value(fileName, InvalidIndex); }

private:
	friend class Archive;
	void add(ArcNodeEntryItem *item)
	{
		m_index[item->fileName()] = m_items.size();
		m_items.push_back(item);
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
	FileTypeInfo m_typeInfo;
};

ARC_PLUGIN_NS_END

#endif /* ARCNODEDIRENTRYITEM_H_ */
