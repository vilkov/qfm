#ifndef TEXTEDITDIALOG_H
#define TEXTEDITDIALOG_H

#include <QDialog>
#include <QString>
#include "../../../../uic/ui_texteditdialog.h"


class TextEditDialog : public QDialog
{
    Q_OBJECT

public:
    TextEditDialog(const QString &text, QWidget *parent = 0);
    QString text() const { return ui.plainTextEdit->document()->toPlainText(); }

private:
    Ui::TextEditDialogClass ui;
};

#endif // TEXTEDITDIALOG_H
