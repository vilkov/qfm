#ifndef ARTIFACTSITEM_H_
#define ARTIFACTSITEM_H_

#include <QVariant>
#include "../../../../../basemodel/items/modelitem.h"


class ArtifactsItem : public ModelItem
{
	Q_DISABLE_COPY(ArtifactsItem)

public:
	ArtifactsItem(const qint32 &id, ModelItem *parent = 0);
	~ArtifactsItem();

	const qint32 &id() const { return m_id; }
	virtual Qt::ItemFlags flags(int column) const = 0;
	virtual bool isVirtual() const = 0;

protected:
    qint32 m_id;
};

#endif /* ARTIFACTSITEM_H_ */
