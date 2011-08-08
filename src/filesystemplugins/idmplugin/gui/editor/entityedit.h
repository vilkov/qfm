#ifndef ARTIFACTEDIT_H
#define ARTIFACTEDIT_H

#include <QDialog>
#include <QList>
#include <QVariant>
#include <QModelIndexList>
#include "data/editordata.h"
#include "model/artifactsmodel.h"
#include "model/items/artifactsdelegate.h"
#include "../../../../uic/ui_artifactedit.h"


class ArtifactEdit : public QDialog
{
    Q_OBJECT

public:
    ArtifactEdit(EditorData &data, QWidget *parent = 0, const bool buttonBox = false);
    QModelIndexList selection() const { return ui.treeView->selectionModel()->selectedIndexes(); }

protected:
    void insertItems(ArtifactInstance &instance);
    void insertItems(ArtifactInstance &instance, const QList<qint32> &ids);

private slots:
	void actAddInstance();
	void actDelInstance();

private:
    QList<QVariant> header() const;

private:
    Ui::ArtifactEditClass ui;
    EditorData &m_internal;
    ArtifactsDelegate m_delegate;
    ArtifactsModel m_model;
};

#endif // ARTIFACTEDIT_H
