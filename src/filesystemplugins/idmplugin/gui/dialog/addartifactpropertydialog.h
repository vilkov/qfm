#ifndef ADDARTIFACTPROPERTYDIALOG_H
#define ADDARTIFACTPROPERTYDIALOG_H

#include <QDialog>
#include <QList>
#include "../../artifact/artifact.h"
#include "../../../../uic/ui_addartifactpropertydialog.h"


class AddArtifactPropertyDialog : public QDialog
{
    Q_OBJECT

public:
    AddArtifactPropertyDialog(const QList<Artifact*> &artifacts, QWidget *parent = 0);
    ~AddArtifactPropertyDialog();

    QList<Artifact*>::size_type selectedArtifactIndex() const { return ui.artifactComboBox->currentIndex(); }

private:
    Ui::AddArtifactPropertyDialogClass ui;
};

#endif // ADDARTIFACTPROPERTYDIALOG_H
