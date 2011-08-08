#ifndef ARTIFACTSLISTMODEL_H_
#define ARTIFACTSLISTMODEL_H_

#include <QMap>
#include <QList>
#include <QVariant>
#include "../basemodel/model.h"
#include "../../artifact/artifact.h"


class ArtifactsListModel : public Model
{
	Q_OBJECT

public:
	ArtifactsListModel(QObject *parent = 0);
	~ArtifactsListModel();

	void appendArtifact(Artifact *artifact);
	void appendArtifact(Artifact *artifact, Artifact *property);
	void removeArtifact(Artifact *artifact);
	void removeArtifact(Artifact *artifact, Artifact *property);
	void insertArtifacts(const QList<Artifact*> &artifacts);

private:
	ModelItem *insertArtifact(ModelItem *parent, Artifact* artifact);
	void insertArtifacts(ModelItem *parent, const QList<Artifact*> &artifacts);

private:
	QMap<Artifact*, QList<ModelItem*> > m_items;
};

#endif /* ARTIFACTSLISTMODEL_H_ */
