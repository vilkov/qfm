#ifndef CREATEARTIFACTDIALOG_H
#define CREATEARTIFACTDIALOG_H

#include <QDialog>
#include <QMap>
#include <QString>
#include "../../artifact/artifact.h"
#include "../../../../uic/ui_createartifactdialog.h"


class CreateArtifactDialog : public QDialog
{
    Q_OBJECT

public:
    CreateArtifactDialog(QWidget *parent = 0);

    virtual void accept();
    QString name() const { return ui.nameLineEdit->text(); }
    Artifact::Type type() const { return m_types[ui.typeComboBox->currentText()]; }

private:
    Ui::CreateArtifactDialogClass ui;
    QMap<QString, Artifact::Type> m_types;
};

#endif // CREATEARTIFACTDIALOG_H
