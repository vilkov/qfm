#ifndef ARCNODEENTRYITEM_H_
#define ARCNODEENTRYITEM_H_

#include <QtCore/QDateTime>
#include "arcnodeitem.h"
#include "../../../../de/filesystemfiletypeinfo.h"


ARC_PLUGIN_NS_BEGIN

class ArcNodeEntryItem : public ArcNodeItem
{
public:
	ArcNodeEntryItem(const QString &fileName, qint64 extractedSize, const QDateTime &lastModified, Base *parent = 0);

	/* Base */
	virtual QVariant data(qint32 column, qint32 role) const;

	const QString &fileName() const { return m_fileName; }
	qint64 extractedSize() const { return m_extractedSize; }

private:
	QString m_fileName;
	qint64 m_extractedSize;
	QDateTime m_lastModified;
	FileTypeInfo m_typeInfo;
};

ARC_PLUGIN_NS_END

#endif /* ARCNODEENTRYITEM_H_ */
