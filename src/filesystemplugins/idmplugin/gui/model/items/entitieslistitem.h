#ifndef ARTIFACTSLISTITEM_H_
#define ARTIFACTSLISTITEM_H_

#include "../../../artifact/artifact.h"
#include "../../../../basemodel/items/modelitem.h"


class ArtifactsListItem : public ModelItem
{
	Q_DISABLE_COPY(ArtifactsListItem)

public:
	ArtifactsListItem(Artifact *artifact, ModelItem *parent = 0);
	~ArtifactsListItem();

	virtual int columnCount() const { return 1; }
	virtual QVariant data(int column, int role) const;
	Artifact *artifact() { return m_artifact; }

private:
	Artifact *m_artifact;
};

#endif /* ARTIFACTSLISTITEM_H_ */
