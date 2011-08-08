#ifndef ENTITIESLIST_H
#define ENTITIESLIST_H

#include <QtGui/QWidget>
#include "../storage/idmstorage.h"


#include <QModelIndex>
#include "artifactslisterror.h"
#include "model/artifactslistmodel.h"
#include "../artifactmanager.h"
#include "../../../uic/ui_artifactslist.h"


FILE_SYSTEM_NS_BEGIN

class EntitiesListWidget : public QWidget
{
    Q_OBJECT

public:
    EntitiesListWidget(IdmStorage *storage, QWidget *parent = 0);

public slots:
	void actCreate();
	void actDelete();
	void addTo();
	void delFrom();
	void actOpen(const QModelIndex &index);

private:
    Ui::ArtifactsListClass ui;
    ArtifactManager m_manager;
    ArtifactsListModel m_model;
};

FILE_SYSTEM_NS_END

#endif // ENTITIESLIST_H
