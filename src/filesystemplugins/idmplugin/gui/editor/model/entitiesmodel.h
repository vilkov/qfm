#ifndef ARTIFACTSMODEL_H_
#define ARTIFACTSMODEL_H_

#include <QList>
#include <QVariant>
#include "../../../../basemodel/model.h"


class ArtifactsModel : public Model
{
	Q_OBJECT

public:
	ArtifactsModel(const QList<QVariant> &header, QObject *parent = 0);

    virtual Qt::ItemFlags flags(const QModelIndex &index) const;
    void remove(ModelItem *item);
    void clear();
};

#endif /* ARTIFACTSMODEL_H_ */
