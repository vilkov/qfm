#ifndef ARTIFACTSSINGLEITEM_H_
#define ARTIFACTSSINGLEITEM_H_

#include <QVariant>
#include "artifactsitem.h"


class ArtifactsSingleItem : public ArtifactsItem
{
	Q_DISABLE_COPY(ArtifactsSingleItem)

public:
	ArtifactsSingleItem(const qint32 &id, QVariant &data, ModelItem *parent = 0);

	virtual int columnCount() const { return 1; }
	virtual QVariant data(int column, int role) const;
	virtual QVariant data() const { return m_data; }
	virtual void setData(const QVariant &data) { m_data = data; }
	virtual Qt::ItemFlags flags(int column) const;
	virtual bool isVirtual() const { return false; }

private:
	QVariant &m_data;
};

#endif /* ARTIFACTSSINGLEITEM_H_ */
